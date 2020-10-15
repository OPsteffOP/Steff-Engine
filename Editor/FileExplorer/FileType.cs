using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.FileExplorer
{
    public enum FileType
    {
        FOLDER,
        SHADER_FILE
    }

    public class FileData
    {
        public FileData(FileType fileType, string filePath, object data)
        {
            this.fileType = fileType;
            this.filePath = filePath;
            this.data = data;
        }

        public FileType fileType;
        public string filePath;
        public object data;
    }
}
