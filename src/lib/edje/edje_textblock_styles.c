#include "edje_private.h"

static int
_edje_font_is_embedded(Edje_File *edf, const char *font)
{
   if (!eina_hash_find(edf->fonts, font)) return 0;
   return 1;
}

static char *
_edje_format_parse(const char **s)
{
   const char *p;
   const char *s1 = NULL;
   const char *s2 = NULL;
   Eina_Bool quote = EINA_FALSE;

   p = *s;
   if ((!p) || (*p == 0)) return NULL;
   for (;; )
     {
        if (!s1)
          {
             if (*p != ' ') s1 = p;
             if (*p == 0) break;
          }
        else if (!s2)
          {
             if (*p == '\'')
               {
                  quote = !quote;
               }

             if ((p > *s) && (p[-1] != '\\') && (!quote))
               {
                  if (*p == ' ') s2 = p;
               }
             if (*p == 0) s2 = p;
          }
        p++;
        if (s1 && s2 && (s2 > s1))
          {
             size_t len = s2 - s1;
             char *ret = malloc(len + 1);
             memcpy(ret, s1, len);
             ret[len] = '\0';
             *s = s2;
             return ret;
          }
     }
   *s = p;
   return NULL;
}

static void
_edje_format_reparse(Edje_File *edf, const char *str, Edje_Style_Tag *tag_ret, Eina_Strbuf *result)
{
   char *s2, *item;
   const char *s;

   s = str;
   while ((item = _edje_format_parse(&s)))
     {
        const char *pos = strchr(item, '=');
        if (pos)
          {
             size_t key_len = pos - item;
             const char *key = item;
             const char *val = pos + 1;

             if (!strncmp(key, "font_source", key_len))
               {
                  /* dont allow font sources */
               }
             else if (!strncmp(key, "text_class", key_len))
               {
                  if (tag_ret)
                    tag_ret->text_class = eina_stringshare_add(val);

                  // no need to add text_class tag to style
                  // as evas_textblock_style has no idea about
                  // text_class tag.
                  free(item);
                  continue;
               }
             else if (!strncmp(key, "font_size", key_len))
               {
                  if (tag_ret)
                    tag_ret->font_size = atof(val);
               }
             else if (!strncmp(key, "font", key_len)) /* Fix fonts */
               {
                  if (tag_ret)
                    {
                       if (_edje_font_is_embedded(edf, val))
                         {
                            char buffer[120];
                            snprintf(buffer, sizeof(buffer), "edje/fonts/%s", val);
                            tag_ret->font = eina_stringshare_add(buffer);
                         }
                       else
                         {
                            tag_ret->font = eina_stringshare_add(val);
                         }
                    }
               }
             s2 = eina_str_escape(item);
             if (s2)
               {
                  if (eina_strbuf_length_get(result)) eina_strbuf_append(result, " ");
                  eina_strbuf_append(result, s2);
                  free(s2);
               }
          }
        else
          {
             if (eina_strbuf_length_get(result)) eina_strbuf_append(result, " ");
             eina_strbuf_append(result, item);
          }
        free(item);
     }
}


/* Update the given evas_style
 *
 * @param ed The edje containing the given style which need to be updated
 * @param style The style which need to be updated
 * @param force Update the given style forcely or not
 */
void
_edje_textblock_style_update(Edje *ed, Edje_Style *stl, Eina_Bool force)
{
   Eina_List *l;
   Eina_Strbuf *txt = NULL;
   Edje_Style_Tag *tag;
   Edje_Text_Class *tc;
   char *fontset = _edje_fontset_append_escaped, *fontsource = NULL;

   if (!ed->file) return;

   /* Make sure the style is already defined */
   if (!stl->style) return;

   /* we are sure it dosen't have any text_class */
   if (stl->readonly) return;

   /* No need to compute it again and again and again */
   if (!force && stl->cache) return;

   if (!txt)
     txt = eina_strbuf_new();

   if (_edje_fontset_append)
     fontset = eina_str_escape(_edje_fontset_append);
   if (ed->file->fonts)
     fontsource = eina_str_escape(ed->file->path);

   /* Build the style from each tag */
   EINA_LIST_FOREACH(stl->tags, l, tag)
     {
        if (!tag->key) continue;

        /* Add Tag Key */
        eina_strbuf_append(txt, tag->key);
        eina_strbuf_append(txt, "='");

        /* Configure fonts from text class if it exists */
        tc = _edje_text_class_find(ed, tag->text_class);

        /* Add and Handle tag parsed data */
        eina_strbuf_append(txt, tag->value);

        if (!strcmp(tag->key, "DEFAULT"))
          {
             if (fontset)
               {
                  eina_strbuf_append(txt, " font_fallbacks=");
                  eina_strbuf_append(txt, fontset);
               }
             if (fontsource)
               {
                  eina_strbuf_append(txt, " font_source=");
                  eina_strbuf_append(txt, fontsource);
               }
          }
        if (tc && tc->size && !EINA_DBL_EQ(tag->font_size, 0))
          {
             double new_size = _edje_text_size_calc(tag->font_size, tc);
             if (!EINA_DBL_EQ(tag->font_size, new_size))
               {
                  char buffer[32];

                  snprintf(buffer, sizeof(buffer), "%.1f", new_size);
                  eina_strbuf_append(txt, " font_size=");
                  eina_strbuf_append(txt, buffer);
               }
          }
        /* Add font name last to save evas from multiple loads */
        if (tc && tc->font && tag->font)
          {
             const char *f;
             char *sfont = NULL;

             eina_strbuf_append(txt, " font=");

             f = _edje_text_font_get(tag->font, tc->font, &sfont);
             eina_strbuf_append_escaped(txt, f);

             if (sfont) free(sfont);
          }

        eina_strbuf_append(txt, "'");
     }
   if (fontsource) free(fontsource);

   /* Configure the style */
   stl->cache = EINA_TRUE;
   evas_textblock_style_set(stl->style, eina_strbuf_string_get(txt));
   if (txt)
     eina_strbuf_free(txt);
}

/* Update all evas_styles which are in an edje
 *
 * @param ed The edje containing styles which need to be updated
 */
void
_edje_textblock_style_all_update(Edje *ed)
{
   Eina_List *l;
   Edje_Style *stl;

   if (!ed->file) return;

   EINA_LIST_FOREACH(ed->file->styles, l, stl)
      _edje_textblock_style_update(ed, stl, EINA_FALSE);
}

static inline Edje_Style *
_edje_textblock_style_search(Edje *ed, const char *style)
{
   Edje_Style *stl = NULL;
   Eina_List *l;

   if (!style) return NULL;

   EINA_LIST_FOREACH(ed->file->styles, l, stl)
     {
        if ((stl->name) &&
            (stl->name == style || !strcmp(stl->name, style))) break;
        stl = NULL;
     }

   return stl;
}

static inline void
_edje_textblock_style_member_add(Edje *ed, Edje_Style *stl)
{
   Edje_Style_Tag *tag;
   Eina_List *l;

   if (!stl) return;

   EINA_LIST_FOREACH(stl->tags, l, tag)
     {
        if (tag->text_class)
          {
             efl_observable_observer_add(_edje_text_class_member, tag->text_class, ed->obj);

             /* Newly added text_class member should be updated
                according to the latest text_class's status. */
             _edje_textblock_style_update(ed, stl, EINA_TRUE);
          }
     }
}

void
_edje_textblock_styles_add(Edje *ed, Edje_Real_Part *ep)
{
   Edje_Part *pt = ep->part;
   Edje_Part_Description_Text *desc;
   Edje_Style *stl = NULL;
   const char *style;
   unsigned int i;

   if (pt->type != EDJE_PART_TYPE_TEXTBLOCK) return;

   /* if text class exists in the textblock styles for this part,
      add the edje to the tc member list */
   desc = (Edje_Part_Description_Text *)pt->default_desc;
   style = edje_string_get(&desc->text.style);
   stl = _edje_textblock_style_search(ed, style);
   _edje_textblock_style_member_add(ed, stl);

   /* If any other classes exist add them */
   for (i = 0; i < pt->other.desc_count; ++i)
     {
        desc = (Edje_Part_Description_Text *)pt->other.desc[i];
        style = edje_string_get(&desc->text.style);
        stl = _edje_textblock_style_search(ed, style);
        _edje_textblock_style_member_add(ed, stl);
     }
}

void
_edje_textblock_styles_del(Edje *ed, Edje_Part *pt)
{
   Edje_Part_Description_Text *desc;
   Edje_Style *stl = NULL;
   const char *style;
   unsigned int i;

   if (pt->type != EDJE_PART_TYPE_TEXTBLOCK) return;

   desc = (Edje_Part_Description_Text *)pt->default_desc;
   style = edje_string_get(&desc->text.style);
   if (style)
     {
        Eina_List *l;

        EINA_LIST_FOREACH(ed->file->styles, l, stl)
          {
             if ((stl->name) && (!strcmp(stl->name, style))) break;
             stl = NULL;
          }
     }
   if (stl)
     {
        Edje_Style_Tag *tag;
        Eina_List *l;

        EINA_LIST_FOREACH(stl->tags, l, tag)
          {
             if (tag->text_class)
               efl_observable_observer_del(_edje_text_class_member, tag->text_class, ed->obj);
          }
     }

   for (i = 0; i < pt->other.desc_count; ++i)
     {
        desc = (Edje_Part_Description_Text *)pt->other.desc[i];
        style = edje_string_get(&desc->text.style);
        if (style)
          {
             Eina_List *l;

             EINA_LIST_FOREACH(ed->file->styles, l, stl)
               {
                  if ((stl->name) && (!strcmp(stl->name, style))) break;
                  stl = NULL;
               }
          }
        if (stl)
          {
             Edje_Style_Tag *tag;
             Eina_List *l;

             EINA_LIST_FOREACH(stl->tags, l, tag)
               {
                  if (tag->text_class)
                    efl_observable_observer_del(_edje_text_class_member, tag->text_class, ed->obj);
               }
          }
     }
}

/*
 * Finds all the styles having text class tag as text_class and
 * updates them.
 */
void
_edje_textblock_style_all_update_text_class(Edje *ed, const char *text_class)
{
   Eina_List *l, *ll;
   Edje_Style *stl;

   if (!ed->file) return;
   if (!text_class) return;

   EINA_LIST_FOREACH(ed->file->styles, l, stl)
     {
        Edje_Style_Tag *tag;

        if (stl->readonly) continue;

        EINA_LIST_FOREACH(stl->tags, ll, tag)
          {
             if (!tag->text_class) continue;

             if (!strcmp(tag->text_class, text_class))
               {
                  _edje_textblock_style_update(ed, stl, EINA_TRUE);
                  break;
               }
          }
     }
}

/* When we get to here the edje file had been read into memory
 * the name of the style is established as well as the name and
 * data for the tags.  This function will create the Evas_Style
 * object for each style. The style is composed of a base style
 * followed by a list of tags.
 */
void
_edje_textblock_style_parse_and_fix(Edje_File *edf)
{
   Eina_List *l, *ll;
   Edje_Style *stl;
   char *fontset = _edje_fontset_append_escaped;
   Eina_Strbuf *reparseBuffer = eina_strbuf_new();
   Eina_Strbuf *styleBuffer = eina_strbuf_new();
   char *fontsource = edf->fonts ? eina_str_escape(edf->path) : NULL;

   EINA_LIST_FOREACH(edf->styles, l, stl)
     {
        Edje_Style_Tag *tag;

        if (stl->style) break;

        stl->readonly = EINA_TRUE;

        stl->style = evas_textblock_style_new();
        evas_textblock_style_set(stl->style, NULL);

        eina_strbuf_reset(styleBuffer);
        /* Build the style from each tag */
        EINA_LIST_FOREACH(stl->tags, ll, tag)
          {
             if (!tag->key) continue;

             eina_strbuf_reset(reparseBuffer);

             /* Add Tag Key */
             eina_strbuf_append(styleBuffer, tag->key);
             eina_strbuf_append(styleBuffer, "='");

             _edje_format_reparse(edf, tag->value, tag, reparseBuffer);

             /* Add and Handle tag parsed data */
             if (eina_strbuf_length_get(reparseBuffer))
               {
                  if (edf->allocated_strings &&
                      eet_dictionary_string_check(eet_dictionary_get(edf->ef), tag->value) == 0)
                    eina_stringshare_del(tag->value);
                  tag->value = eina_stringshare_add(eina_strbuf_string_get(reparseBuffer));
                  eina_strbuf_append(styleBuffer, tag->value);
               }

             if (!strcmp(tag->key, "DEFAULT"))
               {
                  if (fontset)
                    {
                       eina_strbuf_append(styleBuffer, " font_fallbacks=");
                       eina_strbuf_append(styleBuffer, fontset);
                    }
                  if (fontsource)
                    {
                       eina_strbuf_append(styleBuffer, " font_source=");
                       eina_strbuf_append(styleBuffer, fontsource);
                    }
               }
             eina_strbuf_append(styleBuffer, "'");

             if (tag->text_class) stl->readonly = EINA_FALSE;
          }
        /* Configure the style */
        evas_textblock_style_set(stl->style, eina_strbuf_string_get(styleBuffer));
     }

   if (fontsource) free(fontsource);
   eina_strbuf_free(styleBuffer);
   eina_strbuf_free(reparseBuffer);
}

void
_edje_textblock_style_cleanup(Edje_File *edf)
{
   Edje_Style *stl;

   EINA_LIST_FREE(edf->styles, stl)
     {
        Edje_Style_Tag *tag;

        EINA_LIST_FREE(stl->tags, tag)
          {
             if (edf->allocated_strings &&
                 tag->value &&
                 eet_dictionary_string_check(eet_dictionary_get(edf->ef), tag->value) == 0)
               eina_stringshare_del(tag->value);
             if (edf->free_strings)
               {
                  if (tag->key) eina_stringshare_del(tag->key);
/*                FIXME: Find a proper way to handle it. */
                  if (tag->text_class) eina_stringshare_del(tag->text_class);
                  if (tag->font) eina_stringshare_del(tag->font);
               }
             free(tag);
          }
        if (edf->free_strings && stl->name) eina_stringshare_del(stl->name);
        if (stl->style) evas_textblock_style_free(stl->style);
        free(stl);
     }
}

