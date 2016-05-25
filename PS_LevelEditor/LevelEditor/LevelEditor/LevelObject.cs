using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class LevelObject
    {
        int type = -1;
        VEC3F pos;
        int arryPos = -1;

        public int Type
        {
            get { return type; }
            set { type = value; }
        }

        public VEC3F Pos
        {
            get { return pos; }
            set { pos = value; }
        }

        public int ArrayPos
        {
            get { return arryPos; }
            set { arryPos = value; }
        }

        public override string ToString()
        {
            string name = "";
            switch (type)
            {
                case 0:
                    {
                        name = "Crate";
                    }
                    break;
                case 1:
                    {
                        name = "Cork";
                    }
                    break;
                case 2:
                    {
                        name = "Plug";
                    }
                    break;
                case 3:
                    {
                        name = "Pudge";
                    }
                    break;
                case 4:
                    {
                        name = "SlimeMonster";
                    }
                    break;
            }
            return name;
        }
    }
}
