using Editor.Components.Types;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Components
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    struct ExposedVariableData
    {
        public ExposedVariableData(String name, EditorType editorType, VariableAccessibility accessibility, VariableType type, IntPtr pData)
        {
            this.name = name;
            this.editorType = editorType;
            this.accessibility = accessibility;
            this.type = type;
            this.pData = pData;
        }

        public String name;
        public EditorType editorType;
        public VariableAccessibility accessibility;
        public VariableType type;
        public IntPtr pData;

        public object GetData()
        {
            return GetData(pData, type);
        }

        public static object GetData(IntPtr ptr, VariableType type)
        {
            switch (type)
            {
                case VariableType.BOOL:
                    return GetBool(ptr);
                case VariableType.CHAR:
                case VariableType.SIGNED_CHAR:
                case VariableType.UNSIGNED_CHAR:
                    return GetChar(ptr);
                case VariableType.CHAR16_T:
                case VariableType.SHORT:
                case VariableType.SIGNED_SHORT:
                    return GetShort(ptr);
                case VariableType.UNSIGNED_SHORT:
                    return GetUnsignedShort(ptr);
                case VariableType.CHAR32_T:
                case VariableType.INT:
                case VariableType.SIGNED_INT:
                case VariableType.LONG:
                case VariableType.SIGNED_LONG:
                    return GetInt(ptr);
                case VariableType.UNSIGNED_INT:
                case VariableType.UNSIGNED_LONG:
                    return GetUnsignedInt(ptr);
                case VariableType.LONG_LONG:
                case VariableType.SIGNED_LONG_LONG:
                    return GetLongLong(ptr);
                case VariableType.UNSIGNED_LONG_LONG:
                    return GetUnsignedLongLong(ptr);
                case VariableType.FLOAT:
                    return GetFloat(ptr);
                case VariableType.DOUBLE:
                    return GetDouble(ptr);
                case VariableType.STRING:
                    return GetString(ptr);
                case VariableType.DIRECTX_XMFLOAT3:
                case VariableType.XMFLOAT3:
                    return GetVec3(ptr);
                default:
                    return null;
            }
        }

        public static String SetData(IntPtr ptr, VariableType type, String data)
        {
            switch (type)
            {
                case VariableType.BOOL:
                    ExposedVariableData.SetBool(ptr, Convert.ToBoolean(data));
                    return data;
                case VariableType.CHAR:
                case VariableType.SIGNED_CHAR:
                case VariableType.UNSIGNED_CHAR:
                    ExposedVariableData.SetChar(ptr, char.Parse(data));
                    return data;
                case VariableType.CHAR16_T:
                    ExposedVariableData.SetShort(ptr, short.Parse(data));
                    return data;
                case VariableType.SHORT:
                case VariableType.SIGNED_SHORT:
                {
                    short value;
                    if (short.TryParse(data, out value))
                    {
                        ExposedVariableData.SetShort(ptr, value);
                        return data;
                    }

                    return ExposedVariableData.GetShort(ptr).ToString();
                }
                case VariableType.UNSIGNED_SHORT:
                {
                    ushort value;
                    if (ushort.TryParse(data, out value))
                    {
                        ExposedVariableData.SetUnsignedShort(ptr, value);
                        return data;
                    }

                    return ExposedVariableData.GetUnsignedShort(ptr).ToString();
                }
                case VariableType.CHAR32_T:
                case VariableType.INT:
                case VariableType.SIGNED_INT:
                {
                    int value;
                    if (int.TryParse(data, out value))
                    {
                        ExposedVariableData.SetInt(ptr, value);
                        return data;
                    }

                    return ExposedVariableData.GetInt(ptr).ToString();
                }
                case VariableType.UNSIGNED_INT:
                {
                    uint value;
                    if (uint.TryParse(data, out value))
                    {
                        ExposedVariableData.SetUnsignedInt(ptr, value);
                        return data;
                    }

                    return ExposedVariableData.GetUnsignedInt(ptr).ToString();
                }
                case VariableType.LONG:
                case VariableType.SIGNED_LONG:
                {
                    int value;
                    if (int.TryParse(data, out value))
                    {
                        ExposedVariableData.SetLong(ptr, value);
                        return data;
                    }

                    return ExposedVariableData.GetLong(ptr).ToString();
                }
                case VariableType.UNSIGNED_LONG:
                {
                    uint value;
                    if (uint.TryParse(data, out value))
                    {
                        ExposedVariableData.SetUnsignedLong(ptr, value);
                        return data;
                    }

                    return ExposedVariableData.GetUnsignedLong(ptr).ToString();
                }
                case VariableType.LONG_LONG:
                case VariableType.SIGNED_LONG_LONG:
                {
                    long value;
                    if (long.TryParse(data, out value))
                    {
                        ExposedVariableData.SetLongLong(ptr, value);
                        return data;
                    }

                    return ExposedVariableData.GetLongLong(ptr).ToString();
                }
                case VariableType.UNSIGNED_LONG_LONG:
                {
                    ulong value;
                    if (ulong.TryParse(data, out value))
                    {
                        ExposedVariableData.SetUnsignedLongLong(ptr, value);
                        return data;
                    }
                    
                    return ExposedVariableData.GetUnsignedLongLong(ptr).ToString();
                }
                case VariableType.FLOAT:
                {
                    float value;
                    if (float.TryParse(data, out value))
                    {
                        ExposedVariableData.SetFloat(ptr, value);
                        return data;
                    }

                    return ExposedVariableData.GetFloat(ptr).ToString();
                }
                case VariableType.DOUBLE:
                {
                    double value;
                    if (double.TryParse(data, out value))
                    {
                        ExposedVariableData.SetDouble(ptr, value);
                        return data;
                    }

                    return ExposedVariableData.GetDouble(ptr).ToString();
                }
                case VariableType.STRING:
                {
                    ExposedVariableData.SetString(ptr, data);
                    return data;
                }
            }

            return null;
        }

        // Get data
        public static bool GetBool(IntPtr ptr)
        {
            return Convert.ToBoolean(Marshal.ReadByte(ptr));
        }

        public static char GetChar(IntPtr ptr)
        {
            return Convert.ToChar(Marshal.ReadByte(ptr));
        }

        public static short GetShort(IntPtr ptr)
        {
            return Marshal.ReadInt16(ptr);
        }

        public static ushort GetUnsignedShort(IntPtr ptr)
        {
            return Convert.ToUInt16(Marshal.ReadInt16(ptr));
        }

        public static int GetInt(IntPtr ptr)
        {
            return Marshal.ReadInt32(ptr);
        }

        public static uint GetUnsignedInt(IntPtr ptr)
        {
            return Convert.ToUInt32(Marshal.ReadInt32(ptr));
        }

        public static long GetLong(IntPtr ptr)
        {
            return Marshal.ReadInt32(ptr);
        }

        public static ulong GetUnsignedLong(IntPtr ptr)
        {
            return Convert.ToUInt32(Marshal.ReadInt32(ptr));
        }

        public static long GetLongLong(IntPtr ptr)
        {
            return Marshal.ReadInt64(ptr);
        }

        public static ulong GetUnsignedLongLong(IntPtr ptr)
        {
            return Convert.ToUInt64(Marshal.ReadInt64(ptr));
        }

        public static float GetFloat(IntPtr ptr)
        {
            return BitConverter.ToSingle(BitConverter.GetBytes(Marshal.ReadInt32(ptr)), 0);
        }

        public static double GetDouble(IntPtr ptr)
        {
            return BitConverter.Int64BitsToDouble(Marshal.ReadInt64(ptr));
        }

        public static string GetString(IntPtr ptr)
        {
            return Marshal.PtrToStringAnsi(Marshal.ReadIntPtr(ptr));
        }

        public static Vec3 GetVec3(IntPtr ptr)
        {
            //return Marshal.PtrToStructure<Vec3>(ptr);
            return new Vec3(GetFloat(ptr), GetFloat(IntPtr.Add(ptr, 4)), GetFloat(IntPtr.Add(ptr, 8)));
        }

        // Set data
        public static void SetBool(IntPtr ptr, bool value)
        {
            Marshal.WriteByte(ptr, Convert.ToByte(value));
        }

        public static void SetChar(IntPtr ptr, char value)
        {
            Marshal.WriteByte(ptr, Convert.ToByte(value));
        }

        public static void SetShort(IntPtr ptr, short value)
        {
            Marshal.WriteInt16(ptr, value);
        }

        public static void SetUnsignedShort(IntPtr ptr, ushort value)
        {
            Marshal.WriteInt16(ptr, (short)value);
        }

        public static void SetInt(IntPtr ptr, int value)
        {
            Marshal.WriteInt32(ptr, value);
        }

        public static void SetUnsignedInt(IntPtr ptr, uint value)
        {
            Marshal.WriteInt32(ptr, (int)value);
        }

        public static void SetLong(IntPtr ptr, long value)
        {
            Marshal.WriteInt32(ptr, (int)value);
        }

        public static void SetUnsignedLong(IntPtr ptr, ulong value)
        {
            Marshal.WriteInt32(ptr, (int)value);
        }

        public static void SetLongLong(IntPtr ptr, long value)
        {
            Marshal.WriteInt64(ptr, value);
        }

        public static void SetUnsignedLongLong(IntPtr ptr, ulong value)
        {
            Marshal.WriteInt64(ptr, (long)value);
        }

        public static void SetFloat(IntPtr ptr, float value)
        {
            Marshal.WriteInt32(ptr, BitConverter.ToInt32(BitConverter.GetBytes(value), 0));
        }

        public static void SetDouble(IntPtr ptr, double value)
        {
            Marshal.WriteInt64(ptr, BitConverter.ToInt64(BitConverter.GetBytes(value), 0));
        }

        public static void SetString(IntPtr ptr, string value)
        {
            IntPtr strPtr = Marshal.StringToHGlobalAnsi(value);
            Marshal.WriteIntPtr(ptr, strPtr);
        }

        public static void SetVec3(IntPtr ptr, Vec3 value)
        {
            Marshal.StructureToPtr(value, ptr, false);
        }
    }
}
