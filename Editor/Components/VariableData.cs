﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Components
{
	public enum VariableType : ulong // values are the same as std::hash<std::string>{}("variable type name")
	{
		BOOL = 14785269867199075517,

		CHAR = 17483980429552467645,
		SIGNED_CHAR = 15848939444642836855,
		UNSIGNED_CHAR = 17993809043015555046,
		CHAR16_T = 16898841455427775885,
		CHAR32_T = 6140542466990160179,

		SHORT = 17767075776831802709,
		SIGNED_SHORT = 12795206926027596231,
		UNSIGNED_SHORT = 2856468399610700180,

		INT = 3143511548502526014,
		SIGNED_INT = 9846009083299361600,
		UNSIGNED_INT = 13451932124814009803,

		LONG = 14837330719131395891,
		SIGNED_LONG = 12808395840240653977,
		UNSIGNED_LONG = 12153588569745951996,

		LONG_LONG = 18439726635740412007,
		SIGNED_LONG_LONG = 2183365545629165253,
		UNSIGNED_LONG_LONG = 14026568137649808738,

		FLOAT = 11532138274943533413,
		DOUBLE = 11567507311810436776,

		STRING = 6086356466859153974, // (const char*)

		DIRECTX_XMFLOAT3 = 15564571411128910576,
		XMFLOAT3 = 2240813230205538693,
	};

	public enum VariableAccessibility
	{
		READONLY,
		READWRITE
	};
}
