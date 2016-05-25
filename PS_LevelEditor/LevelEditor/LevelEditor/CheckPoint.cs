using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class CheckPoint
    {
        VEC3F position;
        string nextLevelName;

        public string NextLevelName
        {
            get { return nextLevelName; }
            set { nextLevelName = value; }
        }

        public VEC3F Position
        {
            get { return position; }
            set { position = value; }
        }
    }
}
