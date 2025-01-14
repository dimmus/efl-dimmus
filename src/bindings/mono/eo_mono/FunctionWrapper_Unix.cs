/*
 * Copyright 2019 by its authors. See AUTHORS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !WIN32

using System;
using System.Runtime.InteropServices;

namespace Efl.Eo
{

internal static partial class FunctionInterop
{
    [DllImport(efl.Libs.Libdl)]
    private static extern IntPtr dlsym(IntPtr handle, string symbol);

    /// <summary>Loads a function pointer from the given module.
    /// <para>Since EFL 1.23.</para>
    /// </summary>
    /// <param name="nativeLibraryHandle">The module containing the function.</param>
    /// <param name="functionName">The name of the function to search for.</param>
    /// <returns>A function pointer that can be used with delegates.</returns>
    internal static IntPtr LoadFunctionPointer(IntPtr nativeLibraryHandle, string functionName)
    {
        Eina.Log.Debug($"searching {nativeLibraryHandle} for {functionName}");
        var s = FunctionInterop.dlsym(nativeLibraryHandle, functionName);
        Eina.Log.Debug($"searching {nativeLibraryHandle} for {functionName}, result {s}");
        return s;
    }
}

}

#endif