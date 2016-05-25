//////////////////////////////////////////////////////////////////////
//
// Copyright (c) Audiokinetic Inc. 2006-2012. All rights reserved.
//
// Audiokinetic Wwise SDK version, build number and copyright constants.
// These are used by sample projects to display the version and to
// include it in their assembly info. They can also be used by games
// or tools to display the current version and build number of the
// Wwise Sound Engine.
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Text;

namespace AK
{
	namespace Wwise
	{
		public static class Version
		{
			#region Wwise SDK Version - Numeric values

			/// <summary>
			/// Wwise SDK major version
			/// </summary>
            public const int Major = 2012;

			/// <summary>
			/// Wwise SDK minor version
			/// </summary>
            public const int Minor = 1;

			/// <summary>
			/// Wwise SDK sub-minor version
			/// </summary>
            public const int SubMinor = 0;

			/// <summary>
			/// Wwise SDK build number
			/// </summary>
            public const int Build = 4249;
			
			/// <summary>
			/// Wwise SDK build nickname
			/// </summary>
			public const string Nickname = "";

			#endregion Wwise SDK Version - Numeric values

			#region Wwise SDK Version - String values

			/// <summary>
			/// String representing the Wwise SDK version
			/// </summary>
            public static string VersionName
            {
                get
                {
                    if (Nickname.Length == 0)
                        return "v2012.1";
                    else
                        return "v2012.1_" + Nickname;
                }
            }

            /// <summary>
            /// String representing the Wwise SDK version
            /// </summary>
            public const string AssemblyVersion = "2012.1.0.4249";

			/// <summary>
			/// String representing the Wwise SDK copyright notice
			/// </summary>
            public const string CopyrightNotice = "© Audiokinetic Inc. 2006-2012. All rights reserved.";

            #endregion Wwise SDK Version - String values
        }
	}
}