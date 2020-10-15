using Editor.Components;
using Editor.GameObjects;
using Editor.Input;
using Editor.Shaders;
using Editor.Windows;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{
    class Bridge
    {
        private static Bridge m_pInstance = new Bridge();
        private bool m_Disposed = false;

        ~Bridge()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            if (!m_Disposed)
            {
                CleanUp();
                m_Disposed = true;
            }
        }

        public static List<GameObjectData> GetGameObjectsData()
        {
            IntPtr arrayPtr = new IntPtr();
            int arraySize = (int)GetGameObjectsData(out arrayPtr);

            List<GameObjectData> array = new List<GameObjectData>(arraySize);
            for (int i = 0; i < arraySize; ++i)
            {
                IntPtr strStart = Marshal.ReadIntPtr(arrayPtr);
                String name = Marshal.PtrToStringAnsi(strStart);
                uint id = Convert.ToUInt32(Marshal.ReadInt32(IntPtr.Add(arrayPtr, IntPtr.Size)));

                array.Add(new GameObjectData(name, id));
                arrayPtr += IntPtr.Size + 4 + 4; // IntPtr.Size (string), + 4 (unsigned int), + 4 (padding) ==> intptr = 8 bytes, unsigned int = 4 bytes so 4 bytes padding
            }

            return array;
        }

        public static List<uint> GetGameObjectComponents(uint gameObjectID)
        {
            IntPtr arrayPtr = new IntPtr();
            int arraySize = (int)GetGameObjectComponents(gameObjectID, out arrayPtr);

            List<uint> array = new List<uint>(arraySize);
            for (int i = 0; i < arraySize; ++i)
            {
                array.Add(Convert.ToUInt32(Marshal.ReadInt32(arrayPtr)));
                arrayPtr += 4;
            }

            return array;
        }

        public static GameObjectData CreateGameObject()
        {
            IntPtr strPtr = new IntPtr();
            uint gameObjectID = CreateGameObject(out strPtr);

            String gameObjectName = Marshal.PtrToStringAnsi(strPtr);

            return new GameObjectData(gameObjectName, gameObjectID);
        }

        public static List<String> GetComponentNames()
        {
            IntPtr arrayPtr = new IntPtr();
            ulong arraySize = GetComponentNames(out arrayPtr);

            List<String> array = new List<String>((int)arraySize);
            IntPtr advancingArrayPtr = arrayPtr;
            for (int i = 0; i < (int)arraySize; ++i)
            {
                IntPtr strStart = Marshal.ReadIntPtr(advancingArrayPtr);
                array.Add(Marshal.PtrToStringAnsi(strStart));
                advancingArrayPtr += IntPtr.Size;
            }

            FreeComponentNamesArray(1, arrayPtr);

            return array;
        }

        public static KeyValuePair<String, List<ExposedVariableData>> GetComponentData(uint id)
        {
            IntPtr arrayPtr = new IntPtr();
            IntPtr componentNamePtr = new IntPtr();
            int arraySize = (int)GetComponentData(id, out arrayPtr, out componentNamePtr);

            List<ExposedVariableData> array = new List<ExposedVariableData>(arraySize);
            for (int i = 0; i < arraySize; ++i)
            {
                ExposedVariableData data = Marshal.PtrToStructure<ExposedVariableData>(arrayPtr);
                array.Add(data);
                arrayPtr += Marshal.SizeOf(data);
            }

            String componentName = Marshal.PtrToStringAnsi(componentNamePtr);

            return new KeyValuePair<String, List<ExposedVariableData>>(componentName, array);
        }

        public static int CreateComponent(uint gameObjectID, String componentName)
        {
            IntPtr strPtr = Marshal.StringToHGlobalAnsi(componentName);

            int componentID = CreateComponent(gameObjectID, strPtr);

            Marshal.FreeHGlobal(strPtr);
            return componentID;
        }

        public static void VariableChanged(uint componentID, String variableName)
        {
            IntPtr strPtr = Marshal.StringToHGlobalAnsi(variableName);
            VariableChanged(componentID, strPtr);
            Marshal.FreeHGlobal(strPtr);
        }

        public static String CompileShader(String shaderPath, ShaderType type)
        {
            IntPtr pShaderPath = Marshal.StringToHGlobalAnsi(shaderPath);
            IntPtr pErrorMessage = new IntPtr();
            int errorMessageSize = CompileShader(pShaderPath, type, out pErrorMessage);
            Marshal.FreeHGlobal(pShaderPath);

            if (errorMessageSize > 0)
                return Marshal.PtrToStringAnsi(pErrorMessage, errorMessageSize);

            return String.Empty;
        }

        public static uint LoadShader(String shaderName)
        {
            IntPtr pShaderName = Marshal.StringToHGlobalAnsi(shaderName);
            uint shaderID = LoadShader(pShaderName);
            Marshal.FreeHGlobal(pShaderName);

            return shaderID;
        }

        public static uint CreateShaderData(String shaderName, String compiledShaderPath)
        {
            IntPtr pShaderName = Marshal.StringToHGlobalAnsi(shaderName);
            IntPtr pCompiledShaderPath = Marshal.StringToHGlobalAnsi(compiledShaderPath);

            uint shaderID = CreateShaderData(pShaderName, pCompiledShaderPath);

            Marshal.FreeHGlobal(pShaderName);
            Marshal.FreeHGlobal(pCompiledShaderPath);

            return shaderID;
        }

        public static void AddShaderData(uint shaderID, String propertyValue, ShaderType shaderType)
        {
            IntPtr pPropertyValue = Marshal.StringToHGlobalAnsi(propertyValue);
            AddShaderData(shaderID, pPropertyValue, shaderType);
            Marshal.FreeHGlobal(pPropertyValue);
        }

        // Windows 
        [DllImport("user32.dll")]
        public static extern int PeekMessage(out NativeMessage message, IntPtr window, uint filterMin, uint filterMax, uint remove);

        // Initialization & Cleanup
        [DllImport("Engine.dll")]
        private static extern void CleanUp();

        [DllImport("Engine.dll")]
        public static extern void Initialize(IntPtr pWindowHandle, uint windowWidth, uint windowHeight);

        [DllImport("Engine.dll")]
        public static extern void Tick();

        // Debugging
        [DllImport("Engine.dll")]
        public static extern void ToggleDebuggingTools();

        // Input
        [DllImport("Engine.dll")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool ProcessWindowsMessage(IntPtr windowHandle, int msg, IntPtr wParam, IntPtr lParam);

        [DllImport("Engine.dll")]
        public static extern void SetInputEnabled([MarshalAs(UnmanagedType.I1)] bool isEnabled);

        [DllImport("Engine.dll")]
        public static extern void AddKeyInput(ref KeyInput keyInput);

        [DllImport("Engine.dll")]
        public static extern void AddMouseInput(ref MouseInput mouseInput);

        // GameObjects
        [DllImport("Engine.dll")]
        private static extern uint GetGameObjectsData(out IntPtr array);

        [DllImport("Engine.dll")]
        private static extern uint GetGameObjectComponents(uint gameObjectID, out IntPtr array);

        [DllImport("Engine.dll")]
        private static extern uint CreateGameObject(out IntPtr gameObjectName);

        // Components
        [DllImport("Engine.dll")]
        private static extern ulong GetComponentNames(out IntPtr array);

        [DllImport("Engine.dll")]
        private static extern void FreeComponentNamesArray(ulong size, IntPtr array);

        [DllImport("Engine.dll")]
        private static extern ulong GetComponentData(uint id, out IntPtr array, out IntPtr componentName);

        [DllImport("Engine.dll")]
        private static extern int CreateComponent(uint gameObjectID, IntPtr componentName);

        [DllImport("Engine.dll")]
        private static extern void VariableChanged(uint componentID, IntPtr variableName);

        // Shaders
        [DllImport("Engine.dll")]
        private static extern int CompileShader(IntPtr shaderPath, ShaderType type, out IntPtr errorMessage);

        [DllImport("Engine.dll")]
        private static extern uint LoadShader(IntPtr shaderName);

        [DllImport("Engine.dll")]
        private static extern uint CreateShaderData(IntPtr shaderName, IntPtr compiledShaderPath);

        [DllImport("Engine.dll")]
        private static extern void AddShaderData(uint shaderID, IntPtr propertyValue, ShaderType shaderType);
    }

    enum RenderState
    {
        SOLID,
        WIREFRAME,

        end
    }
}
