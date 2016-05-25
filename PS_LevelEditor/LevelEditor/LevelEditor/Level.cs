using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class Level
    {
        List<Hazards> hazards = new List<Hazards>();
        List<LevelObject> objects = new List<LevelObject>();
        List<Light> lights = new List<Light>();

        CheckPoint levelCheckPoint = null;
        int currentHazard = -1;
        string name = "";
        bool connectGooPits = false;
        VEC3F maxKdTree;
        VEC3F minKdTree;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public bool ConnectGooPits
        {
            get { return connectGooPits; }
            set { connectGooPits = value; }
        }

        public int CurrentHazard
        {
            get { return currentHazard; }
            set { currentHazard = value; }
        }

        public List<Hazards> Hazards
        {
            get { return hazards; }
            set { hazards = value; }
        }

        public List<LevelObject> Objects
        {
            get { return objects; }
            set { objects = value; }
        }

        public List<Light> Lights
        {
            get { return lights; }
            set { lights = value; }
        }

        public CheckPoint LevelCheckPoint
        {
            get { return levelCheckPoint; }
            set { levelCheckPoint = value; }
        }

        public VEC3F MinKdTree
        {
            get { return minKdTree; }
            set { minKdTree = value; }
        }

        public VEC3F MaxKdTree
        {
            get { return maxKdTree; }
            set { maxKdTree = value; }
        }

        public override string ToString()
        {
            string LevelName = name;
            LevelName += " -> Next Level : ";

            if (levelCheckPoint.NextLevelName.Length > 0)
            {
                LevelName += levelCheckPoint.NextLevelName;
            }
            else
            {
                LevelName += "NULL";
            }

            return LevelName;
        }
    }
}
