using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;

namespace LevelEditor
{

    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct VEC3F
    {
        public float x; public float y; public float z;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct BASELIGHT
    {
        public float AmbientColorX; public float AmbientColorY; public float AmbientColorZ; public float AmbientColorW;
        public float DiffuseColorX; public float DiffuseColorY; public float DiffuseColorZ; public float DiffuseColorW;
        public float SpecularColorX; public float SpecularColorY; public float SpecularColorZ; public float SpecularColorW;
        public float SpecularIntensity;
        public float SpecularPower;
    };

    public partial class Form1 : Form
    {
        enum HAZARDTYPE { HZ_STEAM_PIPE, HZ_SPAWN_PIPE, HZ_BRIDGE, HZ_BREAKABLE_WALL, HZ_OUTLET, HZ_CRUSHER, HZ_GOO_PIT, HZ_STEAM_INTAKE, HZ_WATER_POOL, 
                          HZ_CRUSHER_BASE, HZ_DOOR_BUTTON, HZ_BOX_BUTTON, HZ_BOX_SPAWN, HZ_DOOR, HZ_CHECK_POINT, HZ_MAX };

        enum OBJECTTYPE { OJ_CRATE, OJ_CORK, OJ_PLUG, OJ_PUDGE, OJ_SLIMEMONSTER, OJ_MAX };

        enum LIGHTTYPE { LT_DIRECTIONAL = 1, LT_POINT = 2, LT_SPOT = 3 }
        
        // import all the renderlink.h functions
        [DllImport("Renderer")]
        public static extern bool _Init(Int32 handle, int width, int height);
        [DllImport("Renderer")]
        public static extern void _Shutdown();
        [DllImport("Renderer")]
        public static extern void _Up();
        [DllImport("Renderer")]
        public static extern void _Down();
        [DllImport("Renderer")]
        public static extern void _Left();
        [DllImport("Renderer")]
        public static extern void _Right();
        [DllImport("Renderer")]
        public static extern void _Forward();
        [DllImport("Renderer")]
        public static extern void _Backward();
        [DllImport("Renderer")]
        public static extern VEC3F _GetCamPos();
        [DllImport("Renderer")]
        public static extern int _GetNumHazards();
        [DllImport("Renderer")]
        public static extern int _GetNumObjects();
        [DllImport("Renderer")]
        public static extern int _GetNumLights();
        [DllImport("Renderer")]
        public static extern VEC3F _GetHazard(int index);
        [DllImport("Renderer")]
        public static extern int _GetHazardType(int index);
        [DllImport("Renderer")]
        public static extern int _GetLightType(int index);
        [DllImport("Renderer")]
        public static extern VEC3F _GetObjectPos(int index);
        [DllImport("Renderer")]
        public static extern int _GetObjectType(int index);
        [DllImport("Renderer")]
        public static extern void _Update(float fElapsedTime);
        [DllImport("Renderer")]
        public static extern void _LoadLevel(char[] szFileName, int length);
        [DllImport("Renderer")]
        public static extern void _SwapLevel(char[] szFileName, int length);
        [DllImport("Renderer")]
        public static extern void _ReloadRenderer();

        [DllImport("Renderer")]
        public static extern void _AddObject(int _nType, VEC3F _vPosition);
        [DllImport("Renderer")]
        public static extern void _RemoveObject(int _nArrayPos);
        [DllImport("Renderer")]
        public static extern void _ChangeObjectType(int _nArrayPos, int _nType);
        [DllImport("Renderer")]
        public static extern void _ChangeObjectPos(int _nArrayPos, VEC3F _vPosition);
        [DllImport("Renderer")]
        public static extern void _RenderDebugSphere(VEC3F _Position);

        [DllImport("Renderer")]
        public static extern void _AddDirectionalLight(BASELIGHT baseLight, VEC3F _vDirection);
        [DllImport("Renderer")]
        public static extern void _AddPointLight(BASELIGHT baseLight, VEC3F _vPosition, VEC3F _vAtten, float _fRadius);
        [DllImport("Renderer")]
        public static extern void _AddSpotLight(BASELIGHT baseLight, VEC3F _vPosition,  VEC3F _vDirection, VEC3F _vAtten, float _fRadius, float _fCutoff, float _fExpo);
        [DllImport("Renderer")]
        public static extern void _RemoveLight(int _nVecPos);
        [DllImport("Renderer")]
        public static extern void _ChangeLightDiffuseColor(int _nVecPos, float _fRed, float _fGreen, float _fBlue, float _fAlpha);
        [DllImport("Renderer")]
        public static extern void _ChangeLightAmbientColor(int _nVecPos, float _fRed, float _fGreen, float _fBlue, float _fAlpha);
        [DllImport("Renderer")]
        public static extern void _ChangeLightSpecularColor(int _nVecPos, float _fRed, float _fGreen, float _fBlue, float _fAlpha);
        [DllImport("Renderer")]
        public static extern void _ChangeLightPosition(int _nVecPos, VEC3F _vPosition);
        [DllImport("Renderer")]
        public static extern void _ChangeLightDirection(int _nVecPos, VEC3F _vDirection);
        [DllImport("Renderer")]
        public static extern void _ChangeLightAtten(int _nVecPos, VEC3F _vAtten);
        [DllImport("Renderer")]
        public static extern void _ChangeLightCutOff(int _nVecPos, float _fCutoff);
        [DllImport("Renderer")]
        public static extern void _ChangeLightExpo(int _nVecPos, float _fExpo);
        [DllImport("Renderer")]
        public static extern void _ChangeLightRadius(int _nVecPos, float _fRadius);
        [DllImport("Renderer")]
        public static extern void _ChangeLightSpecularIntensity(int _nVecPos, float _fIntensity);
        [DllImport("Renderer")]
        public static extern void _ChangeLightSpecularPower(int _nVecPos, float _fPower);
        [DllImport("Renderer")]
        public static extern void _ChangeLightType(int _nVecPos, int _nType);

        [DllImport("Renderer")]
        public static extern float _GetLightRadius (int _nArrayPos);
        [DllImport("Renderer")]
        public static extern VEC3F _GetLightPos (int _nArrayPos);
        [DllImport("Renderer")]
        public static extern VEC3F _GetLightDir (int _nArrayPos);
        [DllImport("Renderer")]
        public static extern VEC3F _GetLightAtten (int _nArrayPos);
        [DllImport("Renderer")]
        public static extern float _GetLightExpo (int _nArrayPos);
        [DllImport("Renderer")]
        public static extern float _GetLightCutOff (int _nArrayPos);
        [DllImport("Renderer")]
        public static extern BASELIGHT _GetLightBase (int _nArrayPos);
        [DllImport("Renderer")]
        public static extern void _ChangeCameraPos(VEC3F _vPos);

        int SplitDistanceConst = 0;
        int CurrentLevelIndex = -1;
       // bool ConnectGooPits = false;

        public Form1()
        {
            InitializeComponent();

            ObjTypeBox.SelectedIndex = 0;
            LightType.SelectedIndex = 0;

            // save the split distance so that if someone tries to move it we can reset it.
            SplitDistanceConst = splitContainer1.SplitterDistance;

            // set all the hazards information group boxs parents to the hazard information tab
            SteamInfo.Parent = HazardInfoTab;
            SteamInfo.Location = new Point(5, 6);

            IntakeInfo.Parent = HazardInfoTab;
            IntakeInfo.Location = new Point(5, 6);

            SpawnInfo.Parent = HazardInfoTab;
            SpawnInfo.Location = new Point(5, 6);

            CrusherInfo.Parent = HazardInfoTab;
            CrusherInfo.Location = new Point(5, 6);

            GooInfo.Parent = HazardInfoTab;
            GooInfo.Location = new Point(5, 6);

            OutletInfo.Parent = HazardInfoTab;
            OutletInfo.Location = new Point(5, 6);

            BridgeInfo.Parent = HazardInfoTab;
            BridgeInfo.Location = new Point(5, 6);

            WaterPoolInfo.Parent = HazardInfoTab;
            WaterPoolInfo.Location = new Point(5, 6);

            BreakableWallInfo.Parent = HazardInfoTab;
            BreakableWallInfo.Location = new Point(5, 6);

            BoxButtonInfo.Parent = HazardInfoTab;
            BoxButtonInfo.Location = new Point(5, 6);

            DoorButtonInfo.Parent = HazardInfoTab;
            DoorButtonInfo.Location = new Point(5, 6);

            DoorInfo.Parent = HazardInfoTab;
            DoorInfo.Location = new Point(5, 6);

            BoxSpawnInfo.Parent = HazardInfoTab;
            BoxSpawnInfo.Location = new Point(5, 6);

            CheckPointInfo.Visible = true;

            DirLightAtt.Parent = LightInfoTab;
            DirLightAtt.Location = new Point(5, 494);

            PointLightAttr.Parent = LightInfoTab;
            PointLightAttr.Location = new Point(5, 494);

            SpotLightAttr.Parent = LightInfoTab;
            SpotLightAttr.Location = new Point(5, 494);
        }

        // the selected tab was changed
        private void MainTabPanel_SelectedIndexChanged(object sender, EventArgs e)
        {
            // make sure there is a current selected level
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            //int selectedTab = MainTabPanel.SelectedIndex;
            ClearAllInformation();

            // unselect the current selected hazard
            ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard = -1;
            List<Hazards> hazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;

            // and make sure all the buttons are not checked
            for (int index = 0; index < hazards.Count; index++)
            {
                hazards[index].Button.Checked = false;
            }
        }

        // over ride update
        public void Update(float fElapsedTime)
        {
            // update the render engine
            _Update(fElapsedTime);

            // dont update if the level index is -1
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            // loop through all the hazards for the current selected level
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            for (int HazardIndex = 0; HazardIndex < LevelHazards.Count; HazardIndex++)
            {
                // update the position of the buttons for that hazard
                VEC3F hazardPos = _GetHazard(LevelHazards[HazardIndex].VectorPos);
                Point pos = new Point();
                pos.X = (int)hazardPos.x;
                pos.Y = (int)hazardPos.y;
                LevelHazards[HazardIndex].Button.Location = pos;
            }

            _RenderDebugSphere(((Level)LevelList.Items[CurrentLevelIndex]).LevelCheckPoint.Position);
            _RenderDebugSphere(((Level)LevelList.Items[CurrentLevelIndex]).MaxKdTree);
            _RenderDebugSphere(((Level)LevelList.Items[CurrentLevelIndex]).MinKdTree);
        }

        // remove all buttons that are currently being displayed and readd the current levels hazard buttons.
        private void ResetDisplayButtons()
        {
            // remove every levels buttons from the display panel
            for (int LevelIndex = 0; LevelIndex < LevelList.Items.Count; LevelIndex++)
            {
                Level level = ((Level)LevelList.Items[LevelIndex]);
                for (int Index = 0; Index < level.Hazards.Count; Index++)
                {
                    level.Hazards[Index].Button.Checked = false;
                    DisplayPanel.Controls.Remove(level.Hazards[Index].Button);
                }
            }

            // add the current selected level buttons to the display panel
            if (CurrentLevelIndex != -1)
            {
                Level level = ((Level)LevelList.Items[CurrentLevelIndex]);
                for (int Index = 0; Index < level.Hazards.Count; Index++)
                {
                    DisplayPanel.Controls.Add(level.Hazards[Index].Button);
                }
            }
        }

        private void ResetObjectList()
        {
            // remove all the objects from the engine
            int LinkObjectCount = _GetNumObjects();
            for (int ObjectIndex = 0; ObjectIndex < _GetNumObjects(); )
            {
                _RemoveObject(ObjectIndex);
            }

            // remove all the objects from the object position list box
            for (int LevelIndex = 0; LevelIndex < LevelList.Items.Count; LevelIndex++)
            {
                Level level = ((Level)LevelList.Items[LevelIndex]);
                for (int Index = 0; Index < level.Objects.Count; Index++)
                {
                    ObjPositionList.Items.Remove(level.Objects[Index]);
                }
            }

            // add the current selected level objects to the object position list and to the engine for rendering
            if (CurrentLevelIndex != -1)
            {
                Level level = ((Level)LevelList.Items[CurrentLevelIndex]);
                for (int Index = 0; Index < level.Objects.Count; Index++)
                {
                    ObjPositionList.Items.Add(level.Objects[Index]);
                    _AddObject(level.Objects[Index].Type, level.Objects[Index].Pos);
                }
            }
        }

        private void ResetLightList()
        {
            // remove all the objects from the engine
            for (int LightIndex = 0; LightIndex < _GetNumLights(); )
            {
                _RemoveLight(LightIndex);
            }

            // remove all the objects from the object position list box
            for (int LevelIndex = 0; LevelIndex < LevelList.Items.Count; LevelIndex++)
            {
                Level level = ((Level)LevelList.Items[LevelIndex]);
                for (int Index = 0; Index < level.Lights.Count; Index++)
                {
                    LevelLights.Items.Remove(level.Lights[Index]);
                }
            }

            // add the current selected level objects to the object position list and to the engine for rendering
            if (CurrentLevelIndex != -1)
            {
                Level level = ((Level)LevelList.Items[CurrentLevelIndex]);
                for (int Index = 0; Index < level.Lights.Count; Index++)
                {
                    LevelLights.Items.Add(level.Lights[Index]);

                    BASELIGHT baselight;
                    baselight.DiffuseColorX = level.Lights[Index].Diffuse.R;
                    baselight.DiffuseColorY = level.Lights[Index].Diffuse.G;
                    baselight.DiffuseColorZ = level.Lights[Index].Diffuse.B;
                    baselight.DiffuseColorW = level.Lights[Index].Diffuse.A;

                    baselight.AmbientColorX = level.Lights[Index].Ambient.R;
                    baselight.AmbientColorY = level.Lights[Index].Ambient.G;
                    baselight.AmbientColorZ = level.Lights[Index].Ambient.B;
                    baselight.AmbientColorW = level.Lights[Index].Ambient.A;

                    baselight.SpecularColorX = level.Lights[Index].Specular.R;
                    baselight.SpecularColorY = level.Lights[Index].Specular.G;
                    baselight.SpecularColorZ = level.Lights[Index].Specular.B;
                    baselight.SpecularColorW = level.Lights[Index].Specular.A;

                    baselight.SpecularIntensity = level.Lights[Index].SpecIntensity;
                    baselight.SpecularPower = level.Lights[Index].Specpower;

                    switch (level.Lights[Index].Type)
                    {
                        case (int)LIGHTTYPE.LT_DIRECTIONAL:
                            {
                                _AddDirectionalLight(baselight, ((DirectionalLight)level.Lights[Index]).Direction);
                            }
                            break;
                        case (int)LIGHTTYPE.LT_POINT:
                            {

                                _AddPointLight(baselight, ((PointLight)level.Lights[Index]).Position, ((PointLight)level.Lights[Index]).Atten, ((PointLight)level.Lights[Index]).Radius);
                            }
                            break;
                        case (int)LIGHTTYPE.LT_SPOT:
                            {
                                _AddSpotLight(baselight, ((SpotLight)level.Lights[Index]).Position, ((SpotLight)level.Lights[Index]).Direction, ((SpotLight)level.Lights[Index]).Atten, ((SpotLight)level.Lights[Index]).Radius, ((SpotLight)level.Lights[Index]).Cutoff, ((SpotLight)level.Lights[Index]).Expo);
                            }
                            break;
                    }
                }
            }
        }

        // clear all information and reload the current selected level
        private void ResetRenderLevel()
        {
            ClearAllInformation();

            if (CurrentLevelIndex == -1)
            {
                _ReloadRenderer();
                return;
            }

            // swap out the level with the current selected level and reset all the infomation
            Level level = (Level)(LevelList.Items[CurrentLevelIndex]);
            _SwapLevel(level.Name.ToCharArray(), level.Name.Length);

            ResetDisplayButtons();
            ResetObjectList();
            ResetLightList();
        }

        // if the order of the levels has changed reset all the levels check point rooms so they load the correct level
        private void CorrectCheckPoints()
        {
            // change who the current and next levels are pointing to
            for (int LevelIndex = 0; LevelIndex < LevelList.Items.Count; LevelIndex++)
            {
                // make sure we have enough levels to do this
                if (LevelIndex + 1 < LevelList.Items.Count)
                {
                    Level CurLevel = (Level)LevelList.Items[LevelIndex];
                    Level NextLevel = (Level)LevelList.Items[LevelIndex+1];
                    CurLevel.LevelCheckPoint.NextLevelName = NextLevel.Name;
                    NextLevel.LevelCheckPoint.NextLevelName = "";
                }
            }

            // make sure to keep what level is selected
            int SelectedLevel = CurrentLevelIndex;
            // a dirty fix to update the list to show the correct names
            for (int LevelIndex = 0; LevelIndex < LevelList.Items.Count; LevelIndex++)
            {
                Level CurLevel = (Level)LevelList.Items[LevelIndex];
                LevelList.Items.RemoveAt(LevelIndex);
                LevelList.Items.Insert(LevelIndex, CurLevel);
            }

            LevelList.SelectedIndex = SelectedLevel;
        }

        #region HandleLevelData
        private void LevelList_SelectedIndexChanged(object sender, EventArgs e)
        {
            // remove all the information
            ClearAllInformation();
            CurrentLevelIndex = LevelList.SelectedIndex;

            if (CurrentLevelIndex == -1)
            {
                _ReloadRenderer();
                return;
            }

            // swap the level with the current selected level
            Level level = (Level)(LevelList.Items[CurrentLevelIndex]);
            _SwapLevel(level.Name.ToCharArray(), level.Name.Length);

            CheckPointPosX.Value = (decimal)level.LevelCheckPoint.Position.x;
            CheckPointPosY.Value = (decimal)level.LevelCheckPoint.Position.y;
            CheckPointPosZ.Value = (decimal)level.LevelCheckPoint.Position.z;

            KDTreeMaxX.Value = (decimal)level.MaxKdTree.x;
            KDTreeMaxY.Value = (decimal)level.MaxKdTree.y;
            KDTreeMaxZ.Value = (decimal)level.MaxKdTree.z;

            KDTreeMinX.Value = (decimal)level.MinKdTree.x;
            KDTreeMinY.Value = (decimal)level.MinKdTree.y;
            KDTreeMinZ.Value = (decimal)level.MinKdTree.z;

            // reset the display and object list box
            ResetDisplayButtons();
            ResetObjectList();
            ResetLightList();

            VEC3F pos;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = -1000.0f;

            for (int Index = 0; Index < ObjPositionList.Items.Count; Index++)
            {
                if (((LevelObject)ObjPositionList.Items[Index]).Type == (int)OBJECTTYPE.OJ_PUDGE)
                {
                    pos.x = ((LevelObject)ObjPositionList.Items[Index]).Pos.x;
                    pos.y = ((LevelObject)ObjPositionList.Items[Index]).Pos.y;
                }
            }

            _ChangeCameraPos(pos);
        }

        private void LevelList_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            CurrentLevelIndex = -1;
            LevelList.SelectedIndex = -1;

            ResetDisplayButtons();
            ResetObjectList();
            ResetLightList();
            ResetRenderLevel();
        }

        // move the current selected level up in the list
        private void LevelUp_Click(object sender, EventArgs e)
        {
           // make sure there is a level selected and the selected level is not the first item
            if (CurrentLevelIndex == -1 || CurrentLevelIndex == 0)
            {
                return;
            }

            // swap the curent level with the level above it in the list
            int CurIndex = CurrentLevelIndex;
            int NextIndex = CurIndex - 1;
            Level NextLevel = (Level)(LevelList.Items[NextIndex]);
            Level CurLevel = (Level)(LevelList.Items[CurIndex]);
            Level Swap = null;

            Swap = CurLevel;
            CurLevel = NextLevel;
            NextLevel = Swap;

            LevelList.Items[CurIndex] = CurLevel;
            LevelList.Items[NextIndex] = NextLevel;

            LevelList.SelectedIndex = NextIndex;

            CorrectCheckPoints();
        }

        // move the current selected level down in the list
        private void LevelDown_Click(object sender, EventArgs e)
        {
            // make sure there is a selected level and the selected level is not the last item
            if (CurrentLevelIndex == -1 || CurrentLevelIndex == LevelList.Items.Count - 1)
            {
                return;
            }

            // swap the current level with the level below it in the list
            int CurIndex = CurrentLevelIndex;
            int PrevIndex = CurIndex + 1;
            Level PrevLevel = (Level)(LevelList.Items[PrevIndex]);
            Level CurLevel = (Level)(LevelList.Items[CurIndex]);
            Level Swap = null;

            Swap = CurLevel;
            CurLevel = PrevLevel;
            PrevLevel = Swap;

            LevelList.Items[CurIndex] = CurLevel;
            LevelList.Items[PrevIndex] = PrevLevel;

            LevelList.SelectedIndex = PrevIndex;

            CorrectCheckPoints();
        }

        // open a level and try to load the data if no data is found load new data using what was loaded from the engine
        private void LevelOpen_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "All Files(*.*)|*.*";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                // get the path for that level
                string _szFilePath = dlg.FileName;
                string[] EndingSplit = _szFilePath.Split('.');

                // make sure try are trying to open a InfiniteHPScene file
                if (EndingSplit[EndingSplit.Length - 1] != "InfiniteHPScene")
                {
                    return;
                }

                // get the level name out of the string path
                string[] PathSplit = _szFilePath.Split('\\');
                string FileName = PathSplit[PathSplit.Length - 1];
                string[] FileNameSplit = FileName.Split('.');

                string LevelName = FileNameSplit[0];

                Level NewLevel = new Level();
                NewLevel.Hazards.Clear();
                NewLevel.Objects.Clear();

                LoadNewLevelData(NewLevel);
                LoadLevel(LevelName, NewLevel);
            }
        }

        private void OpenAll_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "All Files(*.*)|*.*";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                // get the path for that level
                string _szFilePath = dlg.FileName;
                string[] EndingSplit = _szFilePath.Split('.');

                // make sure try are trying to open a InfiniteHPScene file
                if (EndingSplit[EndingSplit.Length - 1] != "InfiniteHPLevelData")
                {
                    return;
                }

                // get the level name out of the string path
                string[] PathSplit = _szFilePath.Split('\\');
                string FullFileName = PathSplit[PathSplit.Length - 1];
                string[] FileNameSplit = FullFileName.Split('.');

                string FileName = FileNameSplit[0];

                string LevelPath = "Resources/LevelData/";
                LevelPath += FileName;
                LevelPath += ".InfiniteHPLevelData";

                if (File.Exists(LevelPath))
                {
                    File.SetAttributes(LevelPath, FileAttributes.Normal);
                }

                FileStream LevelStream = null;
                BinaryReader LevelReader = null;
                byte[] LevelBuffer = null;
                int NumLevels = 0;

                bool LevelDataToLoad = false;
                try
                {
                    LevelStream = new FileStream(LevelPath, FileMode.Open);
                    LevelReader = new BinaryReader(LevelStream);

                    LevelBuffer = new byte[512];
                    LevelReader.Read(LevelBuffer, 0, sizeof(int));
                    NumLevels = BitConverter.ToInt32(LevelBuffer, 0);
                    LevelDataToLoad = true;
                }
                catch
                {
                    LevelDataToLoad = false;
                }

                if (LevelDataToLoad && NumLevels > 0)
                {
                    for (int LevelIndex = 0; LevelIndex < NumLevels; ++LevelIndex)
                    {
                        Level NewLevel = new Level();
                        NewLevel.Hazards.Clear();
                        NewLevel.Objects.Clear();
                        LoadLevelData(NewLevel, LevelReader, LevelBuffer);
                        LoadLevel(NewLevel.Name, NewLevel);
                        FileName = NewLevel.LevelCheckPoint.NextLevelName;
                    }
                }
                else
                {

                }

            }
        }

        private void LoadLevel(string LevelName, Level lvl)
        {
            lvl.Name = LevelName;
            // swap the level with this level to load the correct data on the engine side,  note this will get swaped out
            // for the current selected level at the end of this
            _SwapLevel(lvl.Name.ToCharArray(), lvl.Name.Length);

            // create the path to the trap data and object data files.
            string TrapPath = "Resources/LevelData/TrapData/";
            TrapPath += lvl.Name;
            TrapPath += "_Trap.InfiniteHPHazardData";

            string ObjectPath = "Resources/LevelData/ObjectData/";
            ObjectPath += lvl.Name;
            ObjectPath += "_MoveableObject.InfiniteHPMoveableObjectsData";

            string LightPath = "Resources/LevelData/";
            LightPath += lvl.Name;
            LightPath += ".InfiniteHPLightsData";

            if (File.Exists(TrapPath))
            {
                File.SetAttributes(TrapPath, FileAttributes.Normal);
            }

            if (File.Exists(ObjectPath))
            {
                File.SetAttributes(ObjectPath, FileAttributes.Normal);
            }

            if (File.Exists(LightPath))
            {
                File.SetAttributes(LightPath, FileAttributes.Normal);
            }

            FileStream TrapStream = null;
            BinaryReader TrapReader = null;

            FileStream ObjectStream = null;
            BinaryReader ObjectReader = null;

            FileStream LightStream = null;
            BinaryReader LightReader = null;

            byte[] TrapBuffer = null;
            byte[] ObjectBuffer = null;
            byte[] LightBuffer = null;
            

            int NumHazards = 0;
            int NumObjects = 0;
            int NumLights = 0;

            // try to open the trap data file
            try
            {
                TrapStream = new FileStream(TrapPath, FileMode.Open);
                TrapReader = new BinaryReader(TrapStream);

                TrapBuffer = new byte[512];
                TrapReader.Read(TrapBuffer, 0, sizeof(int));
                NumHazards = BitConverter.ToInt32(TrapBuffer, 0);
            }
            catch
            {
            }

            // try to open the object data file
            try
            {
                ObjectStream = new FileStream(ObjectPath, FileMode.Open);
                ObjectReader = new BinaryReader(ObjectStream);

                ObjectBuffer = new byte[512];
                ObjectReader.Read(ObjectBuffer, 0, sizeof(int));
                NumObjects = BitConverter.ToInt32(ObjectBuffer, 0);
            }
            catch
            {
            }

            try
            {
                LightStream = new FileStream(LightPath, FileMode.Open);
                LightReader = new BinaryReader(LightStream);

                LightBuffer = new byte[512];
                LightReader.Read(ObjectBuffer, 0, sizeof(int));
                NumLights = BitConverter.ToInt32(LightBuffer, 0);
            }
            catch
            {
            }

            // the number of hazards we read in is the same as what the engine read in
            // the load that hazard trap data file
            // else load new hazard data
            if (NumHazards == _GetNumHazards())
            {
                LoadLevelHazardData(lvl, NumHazards, TrapReader, TrapBuffer);
            }
            else
            {
                LoadNewHazardData(lvl);
            }

            // if there are any object in the object file then load that in and 
            // ingore the objects in the engine
            // else load new object data using objects from the engine
            if (NumObjects > 0)
            {
                LoadLevelObjectData(lvl, NumObjects, ObjectReader, ObjectBuffer);
            }
            else
            {
                LoadNewObjectData(lvl);
            }

            if (NumLights > 0)
            {
                LoadLevelLightData(lvl, NumLights, LightReader, LightBuffer);
            }
            else
            {
                LoadNewLightData(lvl);
            }
           

            if (TrapReader != null)
                TrapReader.Close();

            if (TrapStream != null)
                TrapStream.Close();

            if (ObjectReader != null)
                ObjectReader.Close();

            if (ObjectStream != null)
                ObjectStream.Close();

            if (LightStream != null)
                LightStream.Close();

            if (LightReader != null)
                LightReader.Close();

            // add the level to the level list
            LevelList.Items.Add(lvl);

            // reset all information so that it will render the current selected level
            ResetRenderLevel();
            ResetDisplayButtons();
            ResetObjectList();
            ResetLightList();
            CorrectCheckPoints();
        }

        // delete the current selected level
        private void LevelDelete_Click(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            // remove all the data in the display panel and in the object list
            for (int HazardIndex = 0; HazardIndex < ((Level)LevelList.Items[CurrentLevelIndex]).Hazards.Count; HazardIndex++)
            {
                DisplayPanel.Controls.Remove(((Level)LevelList.Items[CurrentLevelIndex]).Hazards[HazardIndex].Button);
            }

            for (int ObjectIndex = 0; ObjectIndex < ((Level)LevelList.Items[CurrentLevelIndex]).Objects.Count; ObjectIndex++)
            {
                ObjPositionList.Items.Remove(((Level)LevelList.Items[CurrentLevelIndex]).Objects[ObjectIndex]);
            }

            int HoldSelectedIndex = CurrentLevelIndex;
            // remove the level from the level list
            LevelList.Items.RemoveAt(CurrentLevelIndex);

            if (LevelList.Items.Count > 0)
            {
                if (HoldSelectedIndex >= LevelList.Items.Count)
                {
                    LevelList.SelectedIndex = LevelList.Items.Count-1;
                    CurrentLevelIndex = LevelList.Items.Count-1;
                }
                else
                {
                    CurrentLevelIndex = HoldSelectedIndex;
                    LevelList.SelectedIndex = HoldSelectedIndex;
                }
            }

            CorrectCheckPoints();

            if (CurrentLevelIndex == -1)
            {
                _ReloadRenderer();
            }
        }
        #endregion

        #region HandleObjectData
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Object List Stuff Below
        private void AddObject_Click(object sender, EventArgs e)
        {
            int SelectedIndex = ObjPositionList.SelectedIndex;
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            // if we are trying to add a pudge
            // make sure no other object in the object list is a pudge
            if (ObjTypeBox.SelectedIndex == (int)OBJECTTYPE.OJ_PUDGE)
            {
                for (int ObjectIndex = 0; ObjectIndex < ObjPositionList.Items.Count; ObjectIndex++)
                {
                    if (((LevelObject)ObjPositionList.Items[ObjectIndex]).Type == (int)OBJECTTYPE.OJ_PUDGE)
                    {
                        DialogResult result2 = MessageBox.Show("You Can not have more then one Pudge Spawn",
                        "To Many Pudges",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Stop);
                        return;
                    }
                }
            }

            // create a new level object
            LevelObject lvlObject = new LevelObject();

            // set the objects type and then its position
            lvlObject.Type = ObjTypeBox.SelectedIndex;

            VEC3F pos;
            pos.x = (float)ObjPosX.Value;
            pos.y = (float)ObjPosY.Value;
            pos.z = (float)ObjPosZ.Value;
            lvlObject.Pos = pos;

            // set its position in the array,  this is used for the engine side
            lvlObject.ArrayPos = ObjPositionList.Items.Count;

            // add the object to the object list, this is to show the user what objects a currently in the levels object list
            ObjPositionList.Items.Add(lvlObject);

            // add the object to the levels list of objects, this is for storing the object data
            ((Level)LevelList.Items[CurrentLevelIndex]).Objects.Add(lvlObject);

            // make sure to select the newly created object
            ObjPositionList.SelectedIndex = ObjPositionList.Items.Count - 1;

            // add the object to the engine so it can be render
            _AddObject(lvlObject.Type, pos);
        }

        // remove an object from the current level and object list
        private void RemoveObject_Click(object sender, EventArgs e)
        {
            // make sure an object is selected
            if (ObjPositionList.SelectedIndex == -1 || CurrentLevelIndex == -1)
            {
                return;
            }

            // get the selected object
            LevelObject lvlObject = (LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex];

            // remove that object from the engine
            _RemoveObject(lvlObject.ArrayPos);

            // remove the object from the current level and the object list
            ((Level)LevelList.Items[CurrentLevelIndex]).Objects.Remove(lvlObject);
            ObjPositionList.Items.Remove(lvlObject);

            // loop through all the objects in the object list
            int ObjCount = ObjPositionList.Items.Count;
            for (int ObjectIndex = 0; ObjectIndex < ObjCount; ObjectIndex++)
            {
                // get the level object and update its position in the array
                lvlObject = (LevelObject)ObjPositionList.Items[ObjectIndex];
                lvlObject.ArrayPos = ObjectIndex;
            }
        }

        // display the new objects information
        private void ObjPositionList_SelectedIndexChanged(object sender, EventArgs e)
        {
            // make sure there is an object selected
            if (ObjPositionList.SelectedIndex == -1)
            {
                return;
            }

            // set the objects type and position using the selected object from the list
            ObjTypeBox.SelectedIndex = ((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Type;
            ObjPosX.Value = (decimal)((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Pos.x;
            ObjPosY.Value = (decimal)((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Pos.y;
            ObjPosZ.Value = (decimal)((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Pos.z;
        }

        private void ObjPositionList_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            ObjPositionList.SelectedIndex = -1;
            ObjTypeBox.SelectedIndex = 0;
            ObjPosX.Value = 0;
            ObjPosY.Value = 0;
            ObjPosZ.Value = 0;
        }

        // the objects type has been changed
       private void ObjTypeBox_SelectedIndexChanged(object sender, EventArgs e)
       {
           int SelectedIndex = ObjPositionList.SelectedIndex;
       
           if (SelectedIndex == -1 || CurrentLevelIndex == -1)
           {
               return;
           }
       
           // check if the type of object to be added is a pudge
           if (ObjTypeBox.SelectedIndex == (int)OBJECTTYPE.OJ_PUDGE && ((LevelObject)ObjPositionList.Items[SelectedIndex]).Type != (int)OBJECTTYPE.OJ_PUDGE)
           {
               // if so then loop through all the objects in the list and make sure this is not another pudge 
               for (int ObjectIndex = 0; ObjectIndex < ObjPositionList.Items.Count; ObjectIndex++)
               {
                   if (((LevelObject)ObjPositionList.Items[ObjectIndex]).Type == (int)OBJECTTYPE.OJ_PUDGE)
                   {
                       DialogResult result2 = MessageBox.Show("You Can not have more then one Pudge Spawn",
                       "To Many Pudges",
                       MessageBoxButtons.OK,
                       MessageBoxIcon.Stop);
                       return;
                   }
               }
           }
       
           // get the type of the new object
           int type = ObjTypeBox.SelectedIndex;
       
           // get the object in the object list
           LevelObject lvlObject = ((LevelObject)ObjPositionList.Items[SelectedIndex]);
           
           // set the objects type
           lvlObject.Type = type;
       
           // update the object in the level list
           ((Level)LevelList.Items[CurrentLevelIndex]).Objects[SelectedIndex] = lvlObject;
       
           // update the object in the object list so that it will display the correct information
           ObjPositionList.Items.RemoveAt (SelectedIndex);
           ObjPositionList.Items.Insert(SelectedIndex, lvlObject);
           ObjPositionList.SelectedIndex = SelectedIndex;
       
           // change the type of object in the engine
           _ChangeObjectType(lvlObject.ArrayPos, type);
       }
       
       // change the position of the object
       private void ObjPosX_ValueChanged(object sender, EventArgs e)
       {
           if (ObjPositionList.SelectedIndex == -1)
           {
               return;
           }
       
           VEC3F pos = ((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Pos;
           pos.x = (float)ObjPosX.Value;
       
           ((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Pos = pos;
       
           _ChangeObjectPos(((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).ArrayPos, pos);
       }
       
       // change the position of the object
       private void ObjPosY_ValueChanged(object sender, EventArgs e)
       {
           if (ObjPositionList.SelectedIndex == -1)
           {
               return;
           }
       
           VEC3F pos = ((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Pos;
           pos.y = (float)ObjPosY.Value;
       
           ((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Pos = pos;
       
           _ChangeObjectPos(((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).ArrayPos, pos);
       }
       
       // change the position of the object
       private void ObjPosZ_ValueChanged(object sender, EventArgs e)
       {
           if (ObjPositionList.SelectedIndex == -1)
           {
               return;
           }
       
           VEC3F pos = ((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Pos;
           pos.z = (float)ObjPosZ.Value;
       
           ((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).Pos = pos;
       
           _ChangeObjectPos(((LevelObject)ObjPositionList.Items[ObjPositionList.SelectedIndex]).ArrayPos, pos);
       }
       //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        #endregion

       #region HandleHazardData
       /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Hazard Stuff Below

        private void Radio_CheckedChanged(object sender, EventArgs e)
        {
            ClearAllInformation();
            RadioButton r = (RadioButton)sender;

            if (CurrentLevelIndex == -1 || r.Checked == false)
            {
                return;
            }

            MainTabPanel.SelectedTab = HazardInfoTab;

            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;

            for (int i = 0; i < LevelHazards.Count; i++)
            {
                if (LevelHazards[i].Button == r)
                {
                    ((Level)LevelList.Items[LevelList.SelectedIndex]).CurrentHazard = i;

                    switch (LevelHazards[i].Type)
                    {
                        case (int)HAZARDTYPE.HZ_SPAWN_PIPE:
                            SpawnButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_STEAM_PIPE:
                            SteamButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_STEAM_INTAKE:
                            SteamIntakeButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_CRUSHER:
                            CrusherButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_GOO_PIT:
                            GooPitButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_BRIDGE:
                            BridgeButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_OUTLET:
                            OutletButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_WATER_POOL:
                            WaterPoolButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_BREAKABLE_WALL:
                            BreakableWallButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_BOX_BUTTON:
                            BoxButtonClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_BOX_SPAWN:
                            BoxSpawnClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_DOOR:
                            DoorClicked(LevelHazards[i]);
                            break;
                        case (int)HAZARDTYPE.HZ_DOOR_BUTTON:
                            DoorButtonClicked(LevelHazards[i]);
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        // set all the informations visible property to false
        void ClearAllInformation()
        {
            SteamInfo.Visible = false;
            IntakeInfo.Visible = false;
            SpawnInfo.Visible = false;
            CrusherInfo.Visible = false;
            GooInfo.Visible = false;
            OutletInfo.Visible = false;
            BridgeInfo.Visible = false;
            WaterPoolInfo.Visible = false;
            BreakableWallInfo.Visible = false;
            BoxButtonInfo.Visible = false;
            DoorButtonInfo.Visible = false;
            DoorInfo.Visible = false;
            BoxSpawnInfo.Visible = false;


            ChannelList.Visible = false;
            ObjsChannelsLabel.Visible = false;
        }

        private void ResetChannelList(Hazards hazard)
        {
            ChannelList.Items.Clear();

            if (hazard.Channel != -1)
            {
                Level lvl = ((Level)LevelList.Items[CurrentLevelIndex]);

                for (int HazardIndex = 0; HazardIndex < lvl.Hazards.Count; HazardIndex++)
                {
                    if (lvl.Hazards[HazardIndex].Channel == hazard.Channel)
                    {
                        ChannelList.Items.Add(lvl.Hazards[HazardIndex]);
                    }
                }
            }
        }

        private void HazardChannel_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazardIndex = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;

            if ((int)(((NumericUpDown)sender).Value) == LevelHazards[CurHazardIndex].Channel)
            {
                ResetChannelList(LevelHazards[CurHazardIndex]);
                return;
            }

            LevelHazards[CurHazardIndex].Channel = (int)(((NumericUpDown)sender).Value);

            switch (LevelHazards[CurHazardIndex].Type)
            {
                case (int)HAZARDTYPE.HZ_GOO_PIT:
                    {
                        if (((Level)LevelList.Items[CurrentLevelIndex]).ConnectGooPits)
                        {
                            GooPitChannelChange();
                        }
                        ResetChannelList(LevelHazards[CurHazardIndex]);
                    }
                    break;
                case (int)HAZARDTYPE.HZ_WATER_POOL:
                    {
                        ResetChannelList(LevelHazards[CurHazardIndex]);
                        WaterPoolChannelChange();
                    }
                    break;
                default:
                    {
                        ResetChannelList(LevelHazards[CurHazardIndex]);
                    }
                    break;
            }
        }

        private void GooPitChannelChange()
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazardIndex = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;

            for (int index = 0; index < ChannelList.Items.Count; ++index)
            {
                if (((Hazards)ChannelList.Items[index]).Type == (int)HAZARDTYPE.HZ_GOO_PIT)
                {
                    ((GooPit)ChannelList.Items[index]).Damage = ((GooPit)LevelHazards[CurHazardIndex]).Damage;
                    ((GooPit)ChannelList.Items[index]).Channel = ((GooPit)LevelHazards[CurHazardIndex]).Channel;
                }
            }
        }

        private void WaterPoolChannelChange()
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazardIndex = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;

            Hazards CurHazard = (LevelHazards[CurHazardIndex]);

            for (int index = 0; index < ChannelList.Items.Count; ++index)
            {
                if (((Hazards)ChannelList.Items[index]) == CurHazard)
                {
                    continue;
                }

                if (((Hazards)ChannelList.Items[index]).Type == (int)HAZARDTYPE.HZ_WATER_POOL)
                {
                    if (((WaterPool)ChannelList.Items[index]).MeshName == "")
                    {
                        ((WaterPool)ChannelList.Items[index]).MeshName = ((WaterPool)CurHazard).MeshName;
                    }

                    if (((WaterPool)CurHazard).MeshName == "")
                    {
                        ((WaterPool)CurHazard).MeshName = ((WaterPool)ChannelList.Items[index]).MeshName;
                        PoolMeshName.Text = ((WaterPool)CurHazard).MeshName;
                    }
                }
            }
        }

        #region SpawnPipeData
        // a spawn button was clicked, display that hazards information
        private void SpawnButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            SpawnInfo.Visible = true;

            SpawnCorkable.Checked = ((SpawnPipe)hazard).Corkable;
            SpawnLimit.Value = (decimal)((SpawnPipe)hazard).MaxSpawns;
            SpawnFreq.Value = (decimal)((SpawnPipe)hazard).Frequency;
            SpawnDir.SelectedIndex = ((SpawnPipe)hazard).Direction;
            HazardChannel.Value = hazard.Channel;

            ResetChannelList(hazard);
        }

        private void SpawnCorkable_CheckedChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SpawnPipe)LevelHazards[CurHazard]).Corkable = ((CheckBox)sender).Checked;
        }

        private void SpawnLimit_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SpawnPipe)LevelHazards[CurHazard]).MaxSpawns = (int)(((NumericUpDown)sender).Value);
        }

        private void SpawnFreq_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SpawnPipe)LevelHazards[CurHazard]).Frequency = (float)(((NumericUpDown)sender).Value);
        }

        private void SpawnDir_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SpawnPipe)LevelHazards[CurHazard]).Direction = (((ComboBox)sender).SelectedIndex);
        }
        #endregion

        #region SteamPipeData
        private void SteamButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            SteamInfo.Visible = true;

            if (!((SteamPipe)hazard).Constant)
            {
                SteamDurationLable.Visible = true;
                SteamDuration.Visible = true;
                SteamFreqLable.Visible = true;
                SteamFreq.Visible = true;
            }
            SteamConstant.Checked = ((SteamPipe)hazard).Constant;
            SteamLength.Value = (decimal)((SteamPipe)hazard).SteamLength;
            SteamDuration.Value = (decimal)((SteamPipe)hazard).Duration;
            SteamFreq.Value = (decimal)((SteamPipe)hazard).Frequency;
            SteamDir.SelectedIndex = ((SteamPipe)hazard).Direction;
            HazardChannel.Value = hazard.Channel;

            ResetChannelList(hazard);
        }

        private void SteamConstant_CheckedChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;

            ((SteamPipe)LevelHazards[CurHazard]).Constant = ((CheckBox)sender).Checked;

            if (((SteamPipe)LevelHazards[CurHazard]).Constant)
            {
                SteamDurationLable.Visible = false;
                SteamDuration.Visible = false;
                SteamFreqLable.Visible = false;
                SteamFreq.Visible = false;
            }
            else
            {
                SteamDurationLable.Visible = true;
                SteamDuration.Visible = true;
                SteamFreqLable.Visible = true;
                SteamFreq.Visible = true;
            }
        }

        private void SteamLength_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SteamPipe)LevelHazards[CurHazard]).SteamLength = (int)(((NumericUpDown)sender).Value);
        }

        private void SteamDuration_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SteamPipe)LevelHazards[CurHazard]).Duration = (float)(((NumericUpDown)sender).Value);
        }

        private void SteamFreq_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SteamPipe)LevelHazards[CurHazard]).Frequency = (float)(((NumericUpDown)sender).Value);
        }

        private void SteamDir_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SteamPipe)LevelHazards[CurHazard]).Direction = (((ComboBox)sender).SelectedIndex);
        }
        #endregion

        #region IntakePipeData
        private void SteamIntakeButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            IntakeInfo.Visible = true;

            IntakeCorkable.Checked = ((SteamIntake)hazard).Corkable;
            IntakeDir.SelectedIndex = ((SteamIntake)hazard).Direction;
            HazardChannel.Value = hazard.Channel;
            FlowPipeName.Text = ((SteamIntake)hazard).FlowName;

            ResetChannelList(hazard);
        }

        private void IntakeCorkable_CheckedChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SteamIntake)LevelHazards[CurHazard]).Corkable = (((CheckBox)sender).Checked);
        }

        private void IntakeDir_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SteamIntake)LevelHazards[CurHazard]).Direction = (((ComboBox)sender).SelectedIndex);
        }

        private void FlowPipeName_TextChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((SteamIntake)LevelHazards[CurHazard]).FlowName = (((TextBox)sender).Text);
        }
        #endregion

        #region CrusherData
        private void CrusherButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            CrusherInfo.Visible = true;

            CrusherLength.Value = (decimal)((Crusher)hazard).Length;
            CrusherDuration.Value = (decimal)((Crusher)hazard).Duration;
            CrusherFreq.Value = (decimal)((Crusher)hazard).Frequency;
            CrusherDir.SelectedIndex = ((Crusher)hazard).Direction;
            HazardChannel.Value = hazard.Channel;

            ResetChannelList(hazard);
        }

        private void CrusherLength_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((Crusher)LevelHazards[CurHazard]).Length = (int)(((NumericUpDown)sender).Value);
        }

        private void CrusherDuration_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((Crusher)LevelHazards[CurHazard]).Duration = (float)(((NumericUpDown)sender).Value);
        }

        private void CrusherFreq_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((Crusher)LevelHazards[CurHazard]).Frequency = (float)(((NumericUpDown)sender).Value);
        }

        private void CrusherDir_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((Crusher)LevelHazards[CurHazard]).Direction = (((ComboBox)sender).SelectedIndex);
        }
        #endregion

        #region GooPitData
        private void GooPitButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            GooInfo.Visible = true;

            ConnectAllPits.Checked = ((Level)LevelList.Items[CurrentLevelIndex]).ConnectGooPits;

            ResetChannelList(hazard);

            GooDamage.Value = (decimal)((GooPit)hazard).Damage;
            HazardChannel.Value = hazard.Channel;
        }

        private void GooDamage_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((GooPit)LevelHazards[CurHazard]).Damage = (float)(((NumericUpDown)sender).Value);

            if (((Level)LevelList.Items[CurrentLevelIndex]).ConnectGooPits)
            {
                Level lvl = ((Level)LevelList.Items[CurrentLevelIndex]);
                float curDamage = ((GooPit)lvl.Hazards[CurHazard]).Damage;

                int count = ChannelList.Items.Count;
                for (int index = 0; index < count; index++)
                {
                    if (((Hazards)ChannelList.Items[index]).Type == (int)HAZARDTYPE.HZ_GOO_PIT)
                    {
                        ((GooPit)ChannelList.Items[index]).Damage = curDamage;
                    }
                }
            }
        }

        private void ConnectAllPits_CheckedChanged(object sender, EventArgs e)
        {
            ((Level)LevelList.Items[CurrentLevelIndex]).ConnectGooPits = ((CheckBox)sender).Checked;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            Level lvl = ((Level)LevelList.Items[CurrentLevelIndex]);

            if (((Level)LevelList.Items[CurrentLevelIndex]).ConnectGooPits)
            {
                List<int> OpenChannels = new List<int>();

                for (int ChannelIndex = 0; ChannelIndex <= HazardChannel.Maximum; ChannelIndex++)
                {
                    int HazardIndex = 0;
                    for (; HazardIndex < lvl.Hazards.Count; HazardIndex++)
                    {
                        if (lvl.Hazards[HazardIndex].Channel == ChannelIndex)
                        {
                            break;
                        }
                    }

                    if (HazardIndex == lvl.Hazards.Count)
                    {
                        OpenChannels.Add(ChannelIndex);
                    }
                }

                for (int MainHazardIndex = 0; MainHazardIndex < lvl.Hazards.Count; ++MainHazardIndex)
                {
                    Hazards MainHazard = lvl.Hazards[MainHazardIndex];

                    if (MainHazard.Type != (int)HAZARDTYPE.HZ_GOO_PIT)
                    {
                        continue;
                    }

                    if (OpenChannels.Count < 1)
                    {
                        break;
                    }

                    if (MainHazard.Channel == -1)
                    {
                        MainHazard.Channel = OpenChannels[0];
                        OpenChannels.RemoveAt(0);
                    }

                    for (int SecondHazardIndex = 0; SecondHazardIndex < lvl.Hazards.Count; SecondHazardIndex++)
                    {
                        Hazards SecondHazard = lvl.Hazards[SecondHazardIndex];

                        if (SecondHazard.Type != (int)HAZARDTYPE.HZ_GOO_PIT)
                        {
                            continue;
                        }

                        if (MainHazard != SecondHazard && MainHazard.Channel != SecondHazard.Channel)
                        {
                            Point Mag = new Point (SecondHazard.Position.X - MainHazard.Position.X, SecondHazard.Position.Y - MainHazard.Position.Y);

                            float distance = (float)Math.Abs ((decimal)Math.Sqrt( (double)((Mag.X * Mag.X) + (Mag.Y * Mag.Y)) ));

                            if (distance <= 120.0f)
                            {
                                SecondHazard.Channel = MainHazard.Channel;
                                MainHazard = SecondHazard;
                                SecondHazardIndex = 0;
                            }
                        }
                    }
                }
            }

            ResetChannelList(lvl.Hazards[CurHazard]);
        }
        #endregion

        #region BridgeData
        private void BridgeButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            BridgeInfo.Visible = true;

            BridgeRotate.Checked = ((Bridge)hazard).RotateClockwise;
            BridgeQuadrant.SelectedIndex = ((Bridge)hazard).Quadrant;
            HazardChannel.Value = hazard.Channel;

            ResetChannelList(hazard);
        }

        private void BridgeQuadrant_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((Bridge)LevelHazards[CurHazard]).Quadrant = (((ComboBox)sender).SelectedIndex);
        }

        private void BridgeRoate_CheckedChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((Bridge)LevelHazards[CurHazard]).RotateClockwise = (((CheckBox)sender).Checked);
        }
        #endregion

        #region OutletData
        private void OutletButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            OutletInfo.Visible = true;

            OutletDir.SelectedIndex = ((OutLet)hazard).Direction;
            HazardChannel.Value = hazard.Channel;

            ResetChannelList(hazard);
        }

        private void OutletDir_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((OutLet)LevelHazards[CurHazard]).Direction = (((ComboBox)sender).SelectedIndex);
        }
        #endregion

        #region WaterPoolData
        private void WaterPoolButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            WaterPoolInfo.Visible = true;

            WaterPoolHeal.Value = (decimal)((WaterPool)hazard).HealAmount;
            HazardChannel.Value = hazard.Channel;
            PoolMeshName.Text = ((WaterPool)hazard).MeshName;

            ResetChannelList(hazard);
        }

        private void WaterPoolHeal_ValueChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;
            ((WaterPool)LevelHazards[CurHazard]).HealAmount = (float)(((NumericUpDown)sender).Value);
        }

        private void PoolMeshName_TextChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;

            if (((WaterPool)LevelHazards[CurHazard]).MeshName == (((TextBox)sender).Text))
            {
                return;
            }

            ((WaterPool)LevelHazards[CurHazard]).MeshName = (((TextBox)sender).Text);

            for (int index = 0; index < ChannelList.Items.Count; ++index)
            {
                if (((Hazards)ChannelList.Items[index]) == ((Hazards)LevelHazards[CurHazard]))
                {
                    continue;
                }

                if (((Hazards)ChannelList.Items[index]).Type == (int)HAZARDTYPE.HZ_WATER_POOL)
                {
                    ((WaterPool)ChannelList.Items[index]).MeshName = ((WaterPool)LevelHazards[CurHazard]).MeshName;
                }
            }
        }
        #endregion

        #region BreakableWallData
        private void BreakableWallButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            BreakableWallInfo.Visible = true;

            HazardChannel.Value = hazard.Channel;

            ResetChannelList(hazard);
        }
        #endregion

        #region BoxButtonData
        private void BoxButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            BoxButtonInfo.Visible = true;

            HazardChannel.Value = hazard.Channel;
            BoxButtonWireName.Text = ((BoxButton)hazard).WireName;

            ResetChannelList(hazard);
        }

        private void BoxButtonWireName_TextChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;

            if (((BoxButton)LevelHazards[CurHazard]).WireName == (((TextBox)sender).Text))
            {
                return;
            }

            ((BoxButton)LevelHazards[CurHazard]).WireName = (((TextBox)sender).Text);
        }
        #endregion

        #region BoxSpawnPipeData
        private void BoxSpawnClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            BoxSpawnInfo.Visible = true;

            HazardChannel.Value = hazard.Channel;

            ResetChannelList(hazard);
        }
        #endregion

        #region DoorButtonData
        private void DoorButtonClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            DoorButtonInfo.Visible = true;

            HazardChannel.Value = hazard.Channel;
            DoorButtonWireName.Text = ((DoorButton)hazard).WireName;

            ResetChannelList(hazard);
        }

        private void DoorButtonWireName_TextChanged(object sender, EventArgs e)
        {
            List<Hazards> LevelHazards = ((Level)LevelList.Items[CurrentLevelIndex]).Hazards;
            int CurHazard = ((Level)LevelList.Items[CurrentLevelIndex]).CurrentHazard;

            if (((DoorButton)LevelHazards[CurHazard]).WireName == (((TextBox)sender).Text))
            {
                return;
            }

            ((DoorButton)LevelHazards[CurHazard]).WireName = (((TextBox)sender).Text);
        }
        #endregion

        #region DoorData
        private void DoorClicked(Hazards hazard)
        {
            ClearAllInformation();

            ChannelList.Visible = true;
            ObjsChannelsLabel.Visible = true;

            DoorInfo.Visible = true;

            HazardChannel.Value = hazard.Channel;

            ResetChannelList(hazard);
        }
        #endregion

        #region CheckPointData
        private void CheckPointPosX_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            VEC3F pos = ((Level)LevelList.Items[CurrentLevelIndex]).LevelCheckPoint.Position;
            pos.x = (float)((NumericUpDown)sender).Value;
            ((Level)LevelList.Items[CurrentLevelIndex]).LevelCheckPoint.Position = pos;
        }

        private void CheckPointPosY_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            VEC3F pos = ((Level)LevelList.Items[CurrentLevelIndex]).LevelCheckPoint.Position;
            pos.y = (float)((NumericUpDown)sender).Value;
            ((Level)LevelList.Items[CurrentLevelIndex]).LevelCheckPoint.Position = pos;
        }

        private void CheckPointPosZ_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            VEC3F pos = ((Level)LevelList.Items[CurrentLevelIndex]).LevelCheckPoint.Position;
            pos.z = (float)((NumericUpDown)sender).Value;
            ((Level)LevelList.Items[CurrentLevelIndex]).LevelCheckPoint.Position = pos;
        }
        #endregion

        #region KDTreeData

        private void KDTreeMaxX_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            VEC3F max = ((Level)LevelList.Items[CurrentLevelIndex]).MaxKdTree;
            max.x = (float)((NumericUpDown)sender).Value;
            ((Level)LevelList.Items[CurrentLevelIndex]).MaxKdTree = max;
        }

        private void KDTreeMaxY_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            VEC3F max = ((Level)LevelList.Items[CurrentLevelIndex]).MaxKdTree;
            max.y = (float)((NumericUpDown)sender).Value;
            ((Level)LevelList.Items[CurrentLevelIndex]).MaxKdTree = max;
        }

        private void KDTreeMaxZ_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            VEC3F max = ((Level)LevelList.Items[CurrentLevelIndex]).MaxKdTree;
            max.z = (float)((NumericUpDown)sender).Value;
            ((Level)LevelList.Items[CurrentLevelIndex]).MaxKdTree = max;
        }

        private void KDTreeMinX_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            VEC3F min = ((Level)LevelList.Items[CurrentLevelIndex]).MinKdTree;
            min.x = (float)((NumericUpDown)sender).Value;
            ((Level)LevelList.Items[CurrentLevelIndex]).MinKdTree = min;
        }

        private void KDTreeMinY_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            VEC3F min = ((Level)LevelList.Items[CurrentLevelIndex]).MinKdTree;
            min.y = (float)((NumericUpDown)sender).Value;
            ((Level)LevelList.Items[CurrentLevelIndex]).MinKdTree = min;
        }

        private void KDTreeMinZ_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1)
            {
                return;
            }

            VEC3F min = ((Level)LevelList.Items[CurrentLevelIndex]).MinKdTree;
            min.z = (float)((NumericUpDown)sender).Value;
            ((Level)LevelList.Items[CurrentLevelIndex]).MinKdTree = min;
        }

        #endregion
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       #endregion

        #region HandleLightData
        // Light Data below:
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void AddLight_Click(object sender, EventArgs e)
        {
            Light newLight = null;

            if (CurrentLevelIndex == -1 || LightType.SelectedIndex == -1)
            {
                return;
            }

            switch (LightType.SelectedIndex + 1)
            {
                case (int)LIGHTTYPE.LT_DIRECTIONAL:
                    {
                        newLight = new DirectionalLight();
                    }
                    break;
                case (int)LIGHTTYPE.LT_POINT:
                    {
                        newLight = new PointLight();
                    }
                    break;
                case (int)LIGHTTYPE.LT_SPOT:
                    {
                        newLight = new SpotLight();
                    }
                    break;
            }

            newLight.VecPos = LevelLights.Items.Count;
            newLight.Type = LightType.SelectedIndex + 1;

            newLight.Diffuse = DiffuseButton.BackColor;
            newLight.Ambient = AmbientButton.BackColor;
            newLight.Specular = SpecularButton.BackColor;

            newLight.Specpower = (float)SpecularPower.Value;
            newLight.SpecIntensity = (float)SpecularIntensity.Value;

            BASELIGHT baselight;
            baselight.DiffuseColorX = newLight.Diffuse.R;
            baselight.DiffuseColorY = newLight.Diffuse.G;
            baselight.DiffuseColorZ = newLight.Diffuse.B;
            baselight.DiffuseColorW = newLight.Diffuse.A;

            baselight.AmbientColorX = newLight.Ambient.R;
            baselight.AmbientColorY = newLight.Ambient.G;
            baselight.AmbientColorZ = newLight.Ambient.B;
            baselight.AmbientColorW = newLight.Ambient.A;

            baselight.SpecularColorX = newLight.Specular.R;
            baselight.SpecularColorY = newLight.Specular.G;
            baselight.SpecularColorZ = newLight.Specular.B;
            baselight.SpecularColorW = newLight.Specular.A;

            baselight.SpecularIntensity = newLight.SpecIntensity;
            baselight.SpecularPower = newLight.Specpower;

            switch (newLight.Type)
            {
                case (int)LIGHTTYPE.LT_DIRECTIONAL:
                    {
                        CreateDirectionalLight((DirectionalLight)newLight);
                        _AddDirectionalLight(baselight, ((DirectionalLight)newLight).Direction);
                    }
                    break;
                case (int)LIGHTTYPE.LT_POINT:
                    {
                        CreatePointLight((PointLight)newLight);
                        _AddPointLight(baselight, ((PointLight)newLight).Position, ((PointLight)newLight).Atten, ((PointLight)newLight).Radius);
                    }
                    break;
                case (int)LIGHTTYPE.LT_SPOT:
                    {
                        CreateSpotLight((SpotLight)newLight);
                        _AddSpotLight(baselight, ((SpotLight)newLight).Position, ((SpotLight)newLight).Direction, ((SpotLight)newLight).Atten, ((SpotLight)newLight).Radius, ((SpotLight)newLight).Cutoff, ((SpotLight)newLight).Expo);
                    }
                    break;
            }

            LevelLights.Items.Add(newLight);
            ((Level)LevelList.Items[CurrentLevelIndex]).Lights.Add(newLight);
            LevelLights.SelectedIndex = newLight.VecPos;
        }

        private void CreateDirectionalLight(DirectionalLight light)
        {
            VEC3F dir;
            dir.x = (float)DirectionalDirX.Value;
            dir.y = (float)DirectionalDirY.Value;
            dir.z = (float)DirectionalDirZ.Value;
            light.Direction = dir;
        }

        private void CreatePointLight(PointLight light)
        {
            VEC3F pos;
            VEC3F atten;
            pos.x = (float)PointLPosX.Value;
            pos.y = (float)PointLPosY.Value;
            pos.z = (float)PointLPosZ.Value;
            atten.x = (float)PointLAttenX.Value;
            atten.y = (float)PointLAttenY.Value;
            atten.z = (float)PointLAttenZ.Value;

            light.Position = pos;
            light.Atten = atten;
            light.Radius = (float)PointLRadius.Value;
        }

        private void CreateSpotLight(SpotLight light)
        {
            VEC3F pos;
            VEC3F dir;
            VEC3F atten;

            pos.x = (float)SpotPosX.Value;
            pos.y = (float)SpotPosY.Value;
            pos.z = (float)SpotPosZ.Value;

            dir.x = (float)SpotDirX.Value;
            dir.y = (float)SpotDirY.Value;
            dir.z = (float)SpotDirZ.Value;

            atten.x = (float)SpotAttenX.Value;
            atten.y = (float)SpotAttenY.Value;
            atten.z = (float)SpotAttenZ.Value;

            light.Position = pos;
            light.Direction = dir;
            light.Atten = atten;

            light.Cutoff = (float)SpotCutOff.Value;
            light.Expo = (float)SpotExponent.Value;
            light.Radius = (float)SpotRadius.Value;
        }

        private void RemoveLight_Click(object sender, EventArgs e)
        {
            if (LevelLights.SelectedIndex == -1 || CurrentLevelIndex == -1)
            {
                return;
            }

            Light light = (Light)LevelLights.Items[LevelLights.SelectedIndex];
            _RemoveLight(light.VecPos);

            ((Level)LevelList.Items[CurrentLevelIndex]).Lights.Remove(light);
            LevelLights.Items.Remove(light);

            int LightCount = LevelLights.Items.Count;
            for (int LightIndex = 0; LightIndex < LightCount; LightIndex++)
            {
                light = (Light)LevelLights.Items[LightIndex];
                light.VecPos = LightIndex;
            }
        }

        private void LevelLights_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }
            Light light = (Light)LevelLights.Items[LevelLights.SelectedIndex];

            

            LightType.SelectedIndex = light.Type-1;
            DiffuseButton.BackColor = light.Diffuse;
            AmbientButton.BackColor = light.Ambient;
            SpecularButton.BackColor = light.Specular;

            SpecularPower.Value = (decimal)light.Specpower;
            SpecularIntensity.Value = (decimal)light.SpecIntensity;

            switch (light.Type)
            {
                case (int)LIGHTTYPE.LT_DIRECTIONAL:
                    {
                        DirLightAtt.Visible = true;
                        PointLightAttr.Visible = false;
                        SpotLightAttr.Visible = false;

                        DirectionalDirX.Value = (decimal)((DirectionalLight)light).Direction.x;
                        DirectionalDirY.Value = (decimal)((DirectionalLight)light).Direction.y;
                        DirectionalDirZ.Value = (decimal)((DirectionalLight)light).Direction.z;
                    }
                    break;
                case (int)LIGHTTYPE.LT_POINT:
                    {
                        _ChangeCameraPos(((PointLight)light).Position);

                        DirLightAtt.Visible = false;
                        PointLightAttr.Visible = true;
                        SpotLightAttr.Visible = false;

                        PointLPosX.Value = (decimal)((PointLight)light).Position.x;
                        PointLPosY.Value = (decimal)((PointLight)light).Position.y;
                        PointLPosZ.Value = (decimal)((PointLight)light).Position.z;

                        PointLAttenX.Value = (decimal)((PointLight)light).Atten.x;
                        PointLAttenY.Value = (decimal)((PointLight)light).Atten.y;
                        PointLAttenZ.Value = (decimal)((PointLight)light).Atten.z;

                        PointLRadius.Value = (decimal)((PointLight)light).Radius;
                    }
                    break;
                case (int)LIGHTTYPE.LT_SPOT:
                    {
                        _ChangeCameraPos(((SpotLight)light).Position);

                        DirLightAtt.Visible = false;
                        PointLightAttr.Visible = false;
                        SpotLightAttr.Visible = true;

                        SpotPosX.Value = (decimal)((SpotLight)light).Position.x;
                        SpotPosY.Value = (decimal)((SpotLight)light).Position.y;
                        SpotPosZ.Value = (decimal)((SpotLight)light).Position.z;

                        SpotDirX.Value = (decimal)((SpotLight)light).Direction.x;
                        SpotDirY.Value = (decimal)((SpotLight)light).Direction.y;
                        SpotDirZ.Value = (decimal)((SpotLight)light).Direction.z;

                        SpotAttenX.Value = (decimal)((SpotLight)light).Atten.x;
                        SpotAttenY.Value = (decimal)((SpotLight)light).Atten.y;
                        SpotAttenZ.Value = (decimal)((SpotLight)light).Atten.z;

                        SpotCutOff.Value = (decimal)((SpotLight)light).Cutoff;
                        SpotExponent.Value = (decimal)((SpotLight)light).Expo;

                        SpotRadius.Value = (decimal)((SpotLight)light).Radius;
                    }
                    break;
            }
        }

        private void LightType_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (LightType.SelectedIndex + 1)
            {
                case (int)LIGHTTYPE.LT_DIRECTIONAL:
                    {
                        DirLightAtt.Visible = true;
                        PointLightAttr.Visible = false;
                        SpotLightAttr.Visible = false;
                    }
                    break;
                case (int)LIGHTTYPE.LT_POINT:
                    {
                        DirLightAtt.Visible = false;
                        PointLightAttr.Visible = true;
                        SpotLightAttr.Visible = false;
                    }
                    break;
                case (int)LIGHTTYPE.LT_SPOT:
                    {
                        DirLightAtt.Visible = false;
                        PointLightAttr.Visible = false;
                        SpotLightAttr.Visible = true;
                    }
                    break;
            }

            if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];
            //int LightVecPos = light.VecPos;

            if (light.Type == LightType.SelectedIndex + 1)
            {
                return;
            }

            switch (LightType.SelectedIndex + 1)
            {
                case (int)LIGHTTYPE.LT_DIRECTIONAL:
                    {
                        light = new DirectionalLight();
                    }
                    break;
                case (int)LIGHTTYPE.LT_POINT:
                    {

                        light = new PointLight();
                        
                    }
                    break;
                case (int)LIGHTTYPE.LT_SPOT:
                    {
                        light = new SpotLight();
                    }
                    break;
            }
            

            light.Type = LightType.SelectedIndex + 1;
            int SelectedIndex = LevelLights.SelectedIndex;

            ((Level)LevelList.Items[CurrentLevelIndex]).Lights[SelectedIndex] = light;

            //LevelLights
            LevelLights.Items.RemoveAt(SelectedIndex);
            LevelLights.Items.Insert(SelectedIndex, light);
            LevelLights.SelectedIndex = SelectedIndex;
            light.VecPos = SelectedIndex;

           _ChangeLightType(light.VecPos, light.Type);
        }

        private void LevelLights_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            LevelLights.SelectedIndex = -1;
            LightType.SelectedIndex = 0;
            DirLightAtt.Visible = false;
            PointLightAttr.Visible = false;
            SpotLightAttr.Visible = false;

            DirectionalDirX.Value = 0;
            DirectionalDirY.Value = 0;
            DirectionalDirZ.Value = 0;

            PointLPosX.Value  = 0;
            PointLPosY.Value  = 0;
            PointLPosZ.Value  = 0;
            PointLAttenX.Value = 0;
            PointLAttenY.Value = 0;
            PointLAttenZ.Value = 0;

            PointLRadius.Value = 0;

            SpotPosX.Value  = 0;
            SpotPosY.Value  = 0;
            SpotPosZ.Value  = 0;
                              
            SpotDirX.Value  = 0;
            SpotDirY.Value  = 0;
            SpotDirZ.Value  = 0;
                              
            SpotAttenX.Value = 0;
            SpotAttenY.Value = 0;
            SpotAttenZ.Value = 0;

            SpotCutOff.Value = 0;
            SpotExponent.Value = 0;
            SpotRadius.Value = 0;
        }

        private void DiffuseButton_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();
            if (DialogResult.OK == dlg.ShowDialog())
            {
                DiffuseButton.BackColor = dlg.Color;

                if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
                {
                    return;
                }

                Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

                if (light.VecPos == -1)
                {
                    return;
                }

                light.Diffuse = dlg.Color;

                _ChangeLightDiffuseColor(light.VecPos, dlg.Color.R, dlg.Color.G, dlg.Color.B, dlg.Color.A);
            }
        }

        private void AmbientButton_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();
            if (DialogResult.OK == dlg.ShowDialog())
            {
                AmbientButton.BackColor = dlg.Color;

                if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
                {
                    return;
                }

                Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

                if (light.VecPos == -1)
                {
                    return;
                }

                light.Ambient = dlg.Color;

                _ChangeLightAmbientColor(light.VecPos, dlg.Color.R, dlg.Color.G, dlg.Color.B, dlg.Color.A);
            }
        }

        private void SpecularButton_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();
            if (DialogResult.OK == dlg.ShowDialog())
            {
                SpecularButton.BackColor = dlg.Color;

                if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
                {
                    return;
                }

                Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

                if (light.VecPos == -1)
                {
                    return;
                }

                light.Specular = dlg.Color;
                _ChangeLightSpecularColor(light.VecPos, dlg.Color.R, dlg.Color.G, dlg.Color.B, dlg.Color.A);
            }
        }

        private void SpecularPower_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            light.Specpower = (float)((NumericUpDown)sender).Value;
            _ChangeLightSpecularPower(light.VecPos, light.Specpower);
        }

        private void SpecularIntensity_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            light.SpecIntensity = (float)((NumericUpDown)sender).Value;
            _ChangeLightSpecularIntensity(light.VecPos, light.SpecIntensity);
        }

        #region DirectionalLightData
        // Directional Light
        //////////////////////////////
        private void DirectionalDirX_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((DirectionalLight)light).Direction;
            pos.x = (float)((NumericUpDown)sender).Value;
            ((DirectionalLight)light).Direction = pos;
            _ChangeLightDirection (light.VecPos, pos);
        }

        private void DirectionalDirY_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((DirectionalLight)light).Direction;
            pos.y = (float)((NumericUpDown)sender).Value;
            ((DirectionalLight)light).Direction = pos;
            _ChangeLightDirection (light.VecPos, pos);
        }

        private void DirectionalDirZ_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((DirectionalLight)light).Direction;
            pos.z = (float)((NumericUpDown)sender).Value;
            ((DirectionalLight)light).Direction = pos;

            _ChangeLightDirection (light.VecPos, pos);
        }
        /////////////////////////////
        #endregion

        #region PointLightData
        // Point Light
        //////////////////////////////
        private void PointLPosX_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentLevelIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((PointLight)light).Position;
            pos.x = (float)((NumericUpDown)sender).Value;
            ((PointLight)light).Position = pos;
            _ChangeLightPosition (light.VecPos, pos);
        }

        private void PointLPosY_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((PointLight)light).Position;
            pos.y = (float)((NumericUpDown)sender).Value;
            ((PointLight)light).Position = pos;
            _ChangeLightPosition (light.VecPos, pos);
        }

        private void PointLPosZ_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((PointLight)light).Position;
            pos.z = (float)((NumericUpDown)sender).Value;
            ((PointLight)light).Position = pos;
            _ChangeLightPosition (light.VecPos, pos);
        }

        private void PointLRadius_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            ((PointLight)light).Radius = (float)((NumericUpDown)sender).Value;

            _ChangeLightRadius(light.VecPos, ((PointLight)light).Radius);
        }

        private void PointLAttenX_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F atten = ((PointLight)light).Atten;
            atten.x = (float)((NumericUpDown)sender).Value;
            ((PointLight)light).Atten = atten;
            _ChangeLightAtten (light.VecPos, atten);
        }

        private void PointLAttenY_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F atten = ((PointLight)light).Atten;
            atten.y = (float)((NumericUpDown)sender).Value;
            ((PointLight)light).Atten = atten;
            _ChangeLightAtten (light.VecPos, atten);
        }

        private void PointLAttenZ_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;

            }
            VEC3F atten = ((PointLight)light).Atten;
            atten.z = (float)((NumericUpDown)sender).Value;
            ((PointLight)light).Atten = atten;
            _ChangeLightAtten (light.VecPos, atten);
        }
        /////////////////////////////
        #endregion

        #region SpotLightData
        // Spot Light
        //////////////////////////////
        private void SpotPosX_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((SpotLight)light).Position;
            pos.x = (float)((NumericUpDown)sender).Value;
            ((SpotLight)light).Position = pos;
            _ChangeLightPosition (light.VecPos, pos);
        }

        private void SpotPosY_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((SpotLight)light).Position;
            pos.y = (float)((NumericUpDown)sender).Value;
            ((SpotLight)light).Position = pos;
            _ChangeLightPosition (light.VecPos, pos);
        }

        private void SpotPosZ_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((SpotLight)light).Position;
            pos.z = (float)((NumericUpDown)sender).Value;
            ((SpotLight)light).Position = pos;
            _ChangeLightPosition (light.VecPos, pos);
        }

        private void SpotDirX_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((SpotLight)light).Direction;
            pos.x = (float)((NumericUpDown)sender).Value;
            ((SpotLight)light).Direction = pos;
            _ChangeLightDirection (light.VecPos, pos);
        }

        private void SpotDirY_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((SpotLight)light).Direction;
            pos.y = (float)((NumericUpDown)sender).Value;
            ((SpotLight)light).Direction = pos;
            _ChangeLightDirection (light.VecPos, pos);
        }

        private void SpotDirZ_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((SpotLight)light).Direction;
            pos.z = (float)((NumericUpDown)sender).Value;
            ((SpotLight)light).Direction = pos;
            _ChangeLightDirection (light.VecPos, pos);
        }

        private void SpotAttenX_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((SpotLight)light).Atten;
            pos.x = (float)((NumericUpDown)sender).Value;
            ((SpotLight)light).Atten = pos;
            _ChangeLightAtten (light.VecPos, pos);
        }

        private void SpotAttenY_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((SpotLight)light).Atten;
            pos.y = (float)((NumericUpDown)sender).Value;
            ((SpotLight)light).Atten = pos;
            _ChangeLightAtten (light.VecPos, pos);
        }

        private void SpotAttenZ_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            VEC3F pos = ((SpotLight)light).Atten;
            pos.z = (float)((NumericUpDown)sender).Value;
            ((SpotLight)light).Atten = pos;
            _ChangeLightAtten (light.VecPos, pos);
        }

        private void SpotCutOff_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            ((SpotLight)light).Cutoff = (float)((NumericUpDown)sender).Value;
            _ChangeLightCutOff(light.VecPos, ((SpotLight)light).Cutoff);
        }

        private void SpotExponent_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            ((SpotLight)light).Expo = (float)((NumericUpDown)sender).Value;
            _ChangeLightExpo(light.VecPos, ((SpotLight)light).Expo);
        }

        private void SpotRadius_ValueChanged(object sender, EventArgs e)
        {
            if (LevelList.SelectedIndex == -1 || LevelLights.SelectedIndex == -1)
            {
                return;
            }

            Light light = ((Level)LevelList.Items[CurrentLevelIndex]).Lights[LevelLights.SelectedIndex];

            if (light.VecPos == -1)
            {
                return;
            }

            ((SpotLight)light).Radius = (float)((NumericUpDown)sender).Value;
            _ChangeLightRadius(light.VecPos, ((SpotLight)light).Radius);
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        #endregion
        #endregion

        #region SaveingAndLoading
        private void SaveAll_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("You are about to save all loaded levels",
                    "Save All Levels",
                    MessageBoxButtons.OKCancel,
                    MessageBoxIcon.Question);

            if (result == DialogResult.OK)
            {
                string LevelPath = "Resources/LevelData/Levels.InfiniteHPLevelData";
                //LevelPath += level.Name;
               //LevelPath += ".InfiniteHPLevelsData";

                if (File.Exists(LevelPath))
                {
                    File.SetAttributes(LevelPath, FileAttributes.Normal);
                }

                FileStream LevelStream = null;
                BinaryWriter LevelWriter = null;

                try
                {
                    LevelStream = new FileStream(LevelPath, FileMode.Open);
                }
                catch
                {
                    LevelStream = new FileStream(LevelPath, FileMode.Create);
                }

                LevelWriter = new BinaryWriter(LevelStream);

                LevelWriter.Write(LevelList.Items.Count);

                for (int LevelIndex = 0; LevelIndex < LevelList.Items.Count; LevelIndex++)
                {
                    Level level = ((Level)LevelList.Items[LevelIndex]);

                    bool PudgeSpawn = false;
                    for (int ObjectIndex = 0; ObjectIndex < level.Objects.Count; ObjectIndex++)
                    {
                        if (((LevelObject)level.Objects[ObjectIndex]).Type == (int)OBJECTTYPE.OJ_PUDGE)
                        {
                            PudgeSpawn = true;
                            break;
                        }
                    }

                    if (PudgeSpawn == false)
                    {
                        string PudgeWarning = "You are about to save ";
                        PudgeWarning += level.Name;
                        PudgeWarning += " without a spawn for pudge. Do you want to continue.";

                        DialogResult Spawnresult = MessageBox.Show(PudgeWarning,
                        "Pudge Spawn Warning",
                        MessageBoxButtons.OKCancel,
                        MessageBoxIcon.Warning);

                        if (Spawnresult == DialogResult.Cancel)
                        {
                            return;
                        }
                    }

                    string TrapPath = "Resources/LevelData/TrapData/";
                    TrapPath += level.Name;
                    TrapPath += "_Trap.InfiniteHPHazardData";

                    string ObjectPath = "Resources/LevelData/ObjectData/";
                    ObjectPath += level.Name;
                    ObjectPath += "_MoveableObject.InfiniteHPMoveableObjectsData";

                    string LightPath = "Resources/LevelData/";
                    LightPath += level.Name;
                    LightPath += ".InfiniteHPLightsData";

                    if (File.Exists(TrapPath))
                    {
                        File.SetAttributes(TrapPath, FileAttributes.Normal);
                    }

                    if (File.Exists(ObjectPath))
                    {
                        File.SetAttributes(ObjectPath, FileAttributes.Normal);
                    }

                    if (File.Exists(LightPath))
                    {
                        File.SetAttributes(LightPath, FileAttributes.Normal);
                    }

                    FileStream TrapStream = null;
                    BinaryWriter TrapWriter = null;

                    FileStream ObjectStream = null;
                    BinaryWriter ObjectWriter = null;

                    FileStream LightStream = null;
                    BinaryWriter LightWriter = null;

                    try
                    {
                        TrapStream = new FileStream(TrapPath, FileMode.Open);
                    }
                    catch
                    {
                        TrapStream = new FileStream(TrapPath, FileMode.Create);
                    }

                    try
                    {
                        ObjectStream = new FileStream(ObjectPath, FileMode.Open);
                    }
                    catch
                    {
                        ObjectStream = new FileStream(ObjectPath, FileMode.Create);
                    }

                    try
                    {
                        LightStream = new FileStream(LightPath, FileMode.Open);
                    }
                    catch
                    {
                        LightStream = new FileStream(LightPath, FileMode.Create);
                    }

                    

                    TrapWriter = new BinaryWriter(TrapStream);
                    ObjectWriter = new BinaryWriter(ObjectStream);
                    LightWriter = new BinaryWriter(LightStream);
                    

                    Level CurLevel = ((Level)LevelList.Items[LevelIndex]);
                    //List<Hazards> LevelHazards = CurLevel.Hazards;
                   // List<LevelObject> LevelObjects = CurLevel.Objects;

                    SaveHazardData(CurLevel, TrapWriter);
                    SaveObjectData(CurLevel, ObjectWriter);
                    SaveLightData(CurLevel, LightWriter);

                    LevelWriter.Write(CurLevel.Name.Length);
                    LevelWriter.Write(CurLevel.Name.ToCharArray());

                    LevelWriter.Write((double)CurLevel.LevelCheckPoint.Position.x);
                    LevelWriter.Write((double)CurLevel.LevelCheckPoint.Position.y);
                    LevelWriter.Write((double)CurLevel.LevelCheckPoint.Position.z);

                    LevelWriter.Write(CurLevel.LevelCheckPoint.NextLevelName.Length);
                    LevelWriter.Write(CurLevel.LevelCheckPoint.NextLevelName.ToCharArray());

                    LevelWriter.Write(CurLevel.MaxKdTree.x);
                    LevelWriter.Write(CurLevel.MaxKdTree.y);
                    LevelWriter.Write(CurLevel.MaxKdTree.z);
                    LevelWriter.Write(CurLevel.MinKdTree.x);
                    LevelWriter.Write(CurLevel.MinKdTree.y);
                    LevelWriter.Write(CurLevel.MinKdTree.z);

                    TrapWriter.Close();
                    TrapStream.Close();

                    ObjectWriter.Close();
                    ObjectStream.Close();

                    LightWriter.Close();
                    LightStream.Close();

                    

                    string Saved = CurLevel.Name;
                    Saved += " has been saved with a next level of ";

                    if (CurLevel.LevelCheckPoint.NextLevelName.Length > 0)
                    {
                        Saved += CurLevel.LevelCheckPoint.NextLevelName;
                    }
                    else
                    {
                        Saved += "NULL";
                    }

                    DialogResult result2 = MessageBox.Show(Saved,
                    "Level Saved",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation);
                }

                LevelWriter.Close();
                LevelStream.Close();
            }
        }

        private void SaveHazardData(Level lvl, BinaryWriter TrapWriter)
        {
            List<Hazards> LevelHazards = lvl.Hazards;

            TrapWriter.Write(LevelHazards.Count);
            for (int HazardIndex = 0; HazardIndex < LevelHazards.Count; HazardIndex++)
            {
                TrapWriter.Write(LevelHazards[HazardIndex].Type);
                TrapWriter.Write(LevelHazards[HazardIndex].Channel);

                switch (LevelHazards[HazardIndex].Type)
                {
                    case (int)HAZARDTYPE.HZ_STEAM_PIPE:
                        {
                            TrapWriter.Write(((SteamPipe)LevelHazards[HazardIndex]).Constant);
                            TrapWriter.Write(((SteamPipe)LevelHazards[HazardIndex]).SteamLength);
                            TrapWriter.Write((double)(((SteamPipe)LevelHazards[HazardIndex]).Duration));
                            TrapWriter.Write((double)(((SteamPipe)LevelHazards[HazardIndex]).Frequency));
                            TrapWriter.Write(((SteamPipe)LevelHazards[HazardIndex]).Direction);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_STEAM_INTAKE:
                        {
                            TrapWriter.Write(((SteamIntake)LevelHazards[HazardIndex]).Corkable);
                            TrapWriter.Write(((SteamIntake)LevelHazards[HazardIndex]).Direction);
                            TrapWriter.Write(((SteamIntake)LevelHazards[HazardIndex]).FlowName.Length);
                            TrapWriter.Write(((SteamIntake)LevelHazards[HazardIndex]).FlowName.ToCharArray());
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_SPAWN_PIPE:
                        {
                            TrapWriter.Write(((SpawnPipe)LevelHazards[HazardIndex]).Corkable);
                            TrapWriter.Write(((SpawnPipe)LevelHazards[HazardIndex]).MaxSpawns);
                            TrapWriter.Write((double)(((SpawnPipe)LevelHazards[HazardIndex]).Frequency));
                            TrapWriter.Write(((SpawnPipe)LevelHazards[HazardIndex]).Direction);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_CRUSHER:
                        {
                            TrapWriter.Write(((Crusher)LevelHazards[HazardIndex]).Length);
                            TrapWriter.Write((double)(((Crusher)LevelHazards[HazardIndex]).Duration));
                            TrapWriter.Write((double)(((Crusher)LevelHazards[HazardIndex]).Frequency));
                            TrapWriter.Write(((Crusher)LevelHazards[HazardIndex]).Direction);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_GOO_PIT:
                        {
                            TrapWriter.Write((double)((GooPit)LevelHazards[HazardIndex]).Damage);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_OUTLET:
                        {
                            TrapWriter.Write(((OutLet)LevelHazards[HazardIndex]).Direction);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_BRIDGE:
                        {
                            TrapWriter.Write(((Bridge)LevelHazards[HazardIndex]).Quadrant);
                            TrapWriter.Write(((Bridge)LevelHazards[HazardIndex]).RotateClockwise);

                        }
                        break;
                    case (int)HAZARDTYPE.HZ_WATER_POOL:
                        {
                            TrapWriter.Write((double)((WaterPool)LevelHazards[HazardIndex]).HealAmount);
                            TrapWriter.Write(((WaterPool)LevelHazards[HazardIndex]).MeshName.Length);
                            TrapWriter.Write(((WaterPool)LevelHazards[HazardIndex]).MeshName.ToCharArray());
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_BOX_BUTTON:
                        {
                            TrapWriter.Write(((BoxButton)LevelHazards[HazardIndex]).WireName.Length);
                            TrapWriter.Write(((BoxButton)LevelHazards[HazardIndex]).WireName.ToCharArray());
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_BOX_SPAWN:
                        {

                        }
                        break;
                    case (int)HAZARDTYPE.HZ_DOOR:
                        {

                        }
                        break;
                    case (int)HAZARDTYPE.HZ_DOOR_BUTTON:
                        {
                            TrapWriter.Write(((DoorButton)LevelHazards[HazardIndex]).WireName.Length);
                            TrapWriter.Write(((DoorButton)LevelHazards[HazardIndex]).WireName.ToCharArray());
                        }
                        break;
                    default:
                        break;
                }
            }

            //TrapWriter.Write((double)lvl.LevelCheckPoint.Position.x);
            //TrapWriter.Write((double)lvl.LevelCheckPoint.Position.y);
            //TrapWriter.Write((double)lvl.LevelCheckPoint.Position.z);

            //TrapWriter.Write(lvl.LevelCheckPoint.NextLevelName.Length);
            //TrapWriter.Write(lvl.LevelCheckPoint.NextLevelName.ToCharArray());

            //TrapWriter.Write(lvl.MaxKdTree.x);
            //TrapWriter.Write(lvl.MaxKdTree.y);
            //TrapWriter.Write(lvl.MaxKdTree.z);
            //TrapWriter.Write(lvl.MinKdTree.x);
            //TrapWriter.Write(lvl.MinKdTree.y);
            //TrapWriter.Write(lvl.MinKdTree.z);
        }

        private void SaveObjectData(Level lvl, BinaryWriter ObjectWriter)
        {
            List<LevelObject> LevelObjects = lvl.Objects;

            ObjectWriter.Write(LevelObjects.Count);
            for (int ObjectIndex = 0; ObjectIndex < LevelObjects.Count; ObjectIndex++)
            {
                ObjectWriter.Write(LevelObjects[ObjectIndex].Type);
                ObjectWriter.Write((double)LevelObjects[ObjectIndex].Pos.x);
                ObjectWriter.Write((double)LevelObjects[ObjectIndex].Pos.y);
                ObjectWriter.Write((double)LevelObjects[ObjectIndex].Pos.z);
            }
        }

        private void SaveLightData(Level lvl, BinaryWriter LightWriter)
        {
            List<Light> LevelLights = lvl.Lights;

            LightWriter.Write(LevelLights.Count);
            for (int LightIndex = 0; LightIndex < LevelLights.Count; LightIndex++)
            {
                LightWriter.Write(LevelLights[LightIndex].Type);

                LightWriter.Write( (float)(LevelLights[LightIndex].Ambient.A/255.0f)  );
                LightWriter.Write( (float)(LevelLights[LightIndex].Ambient.R/255.0f)  );
                LightWriter.Write( (float)(LevelLights[LightIndex].Ambient.G/255.0f)  );
                LightWriter.Write( (float)(LevelLights[LightIndex].Ambient.B/255.0f)  );
                LightWriter.Write( (float)(LevelLights[LightIndex].Diffuse.A/255.0f)  );
                LightWriter.Write( (float)(LevelLights[LightIndex].Diffuse.R/255.0f)  );
                LightWriter.Write( (float)(LevelLights[LightIndex].Diffuse.G/255.0f)  );
                LightWriter.Write( (float)(LevelLights[LightIndex].Diffuse.B/255.0f)  );
                LightWriter.Write( (float)(LevelLights[LightIndex].Specular.A/255.0f) );
                LightWriter.Write( (float)(LevelLights[LightIndex].Specular.R/255.0f) );
                LightWriter.Write( (float)(LevelLights[LightIndex].Specular.G/255.0f) );
                LightWriter.Write( (float)(LevelLights[LightIndex].Specular.B/255.0f) );

                LightWriter.Write((float)LevelLights[LightIndex].SpecIntensity);
                LightWriter.Write((float)LevelLights[LightIndex].Specpower);

                switch (LevelLights[LightIndex].Type)
                {
                    case (int)LIGHTTYPE.LT_DIRECTIONAL:
                        {
                            LightWriter.Write((float)((DirectionalLight)LevelLights[LightIndex]).Direction.x);
                            LightWriter.Write((float)((DirectionalLight)LevelLights[LightIndex]).Direction.y);
                            LightWriter.Write((float)((DirectionalLight)LevelLights[LightIndex]).Direction.z);
                        }
                        break;
                    case (int)LIGHTTYPE.LT_POINT:
                        {
                            LightWriter.Write((float)((PointLight)LevelLights[LightIndex]).Atten.x);
                            LightWriter.Write((float)((PointLight)LevelLights[LightIndex]).Atten.y);
                            LightWriter.Write((float)((PointLight)LevelLights[LightIndex]).Atten.z);

                            LightWriter.Write((float)((PointLight)LevelLights[LightIndex]).Position.x);
                            LightWriter.Write((float)((PointLight)LevelLights[LightIndex]).Position.y);
                            LightWriter.Write((float)((PointLight)LevelLights[LightIndex]).Position.z);

                            LightWriter.Write((float)((PointLight)LevelLights[LightIndex]).Radius);
                        }
                        break;
                    case (int)LIGHTTYPE.LT_SPOT:
                        {
                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Atten.x);
                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Atten.y);
                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Atten.z);

                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Position.x);
                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Position.y);
                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Position.z);

                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Radius);
                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Cutoff);

                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Direction.x);
                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Direction.y);
                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Direction.z);
                            
                            LightWriter.Write((float)((SpotLight)LevelLights[LightIndex]).Expo);
                            
                        }
                        break;
                }
            }
        }

        private void LoadNewLevelData(Level lvl)
        {
            VEC3F checkpos;
            checkpos.x = 0.0f;
            checkpos.y = 0.0f;
            checkpos.z = 0.0f;

            CheckPoint checkPoint = new CheckPoint();
            checkPoint.NextLevelName = "";
            checkPoint.Position = checkpos;
            lvl.LevelCheckPoint = checkPoint;

            VEC3F max, min;
            max.x = 0.0f;
            max.y = 0.0f;
            max.z = 0.0f;

            min.x = 0.0f;
            min.y = 0.0f;
            min.z = 0.0f;

            lvl.MaxKdTree = max;
            lvl.MinKdTree = min;
        }

        private void LoadNewObjectData(Level level)
        {
            int ObjectCount = _GetNumObjects();

            for (int ObjectIndex = 0; ObjectIndex < ObjectCount; ObjectIndex++)
            {
                VEC3F objectPos = _GetObjectPos(ObjectIndex);

                LevelObject lvlObject = new LevelObject();

                lvlObject.Type = _GetObjectType(ObjectIndex);
                lvlObject.Pos = objectPos;
                lvlObject.ArrayPos = ObjectIndex;

                level.Objects.Add(lvlObject);
                ObjPositionList.Items.Add(lvlObject);
            }
        }

        private void LoadNewHazardData(Level level)
        {
            int HazardCount = _GetNumHazards();
            int SpawnPipeCount = 0;
            int SteamPipeCount = 0;
            int SteamIntakeCount = 0;
            int CrusherCount = 0;
            int GooPitCount = 0;
            int BridgeCount = 0;
            int BreakableWallCount = 0;
            int OutletCount = 0;
            int WaterPoolCount = 0;
            int BoxButtonCount = 0;
            int BoxSpawnCount = 0;
            int DoorCount = 0;
            int DoorButtonCount = 0;

            for (int i = 0; i < HazardCount; i++)
            {
                VEC3F hazardPos = _GetHazard(i);
                Point pos = new Point();
                pos.X = (int)hazardPos.x;
                pos.Y = (int)hazardPos.y;

                Hazards hazard = null;

                switch (_GetHazardType(i))
                {
                    case (int)HAZARDTYPE.HZ_SPAWN_PIPE:
                        SpawnPipeCount++;
                        hazard = new SpawnPipe();
                        hazard.Name = "Spawn" + SpawnPipeCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_SPAWN_PIPE;
                        break;
                    case (int)HAZARDTYPE.HZ_STEAM_PIPE:
                        SteamPipeCount++;
                        hazard = new SteamPipe();
                        hazard.Name = "SteamOut" + SteamPipeCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_STEAM_PIPE;
                        break;
                    case (int)HAZARDTYPE.HZ_STEAM_INTAKE:
                        SteamIntakeCount++;
                        hazard = new SteamIntake();
                        hazard.Name = "SteamIntake" + SteamIntakeCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_STEAM_INTAKE;
                        break;
                    case (int)HAZARDTYPE.HZ_CRUSHER:
                        CrusherCount++;
                        hazard = new Crusher();
                        hazard.Name = "Crusher" + CrusherCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_CRUSHER;
                        break;
                    case (int)HAZARDTYPE.HZ_CRUSHER_BASE:
                        hazard = new CrusherBase();
                        hazard.Name = "CrusherBase";
                        hazard.Type = (int)HAZARDTYPE.HZ_CRUSHER_BASE;
                        break;
                    case (int)HAZARDTYPE.HZ_GOO_PIT:
                        GooPitCount++;
                        hazard = new GooPit();
                        hazard.Name = "GooPit" + GooPitCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_GOO_PIT;
                        break;
                    case (int)HAZARDTYPE.HZ_BRIDGE:
                        BridgeCount++;
                        hazard = new Bridge();
                        hazard.Name = "Bridge" + BridgeCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_BRIDGE;
                        break;
                    case (int)HAZARDTYPE.HZ_BREAKABLE_WALL:
                        BreakableWallCount++;
                        hazard = new BreakableWall();
                        hazard.Name = "Breakable Wall" + BreakableWallCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_BREAKABLE_WALL;
                        break;
                    case (int)HAZARDTYPE.HZ_OUTLET:
                        OutletCount++;
                        hazard = new OutLet();
                        hazard.Name = "OutLet" + OutletCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_OUTLET;
                        break;
                    case (int)HAZARDTYPE.HZ_WATER_POOL:
                        WaterPoolCount++;
                        hazard = new WaterPool();
                        hazard.Name = "WaterPool" + WaterPoolCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_WATER_POOL;
                        break;
                    case (int)HAZARDTYPE.HZ_BOX_BUTTON:
                        BoxButtonCount++;
                        hazard = new BoxButton();
                        hazard.Name = "Box Button" + BoxButtonCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_BOX_BUTTON;
                        break;
                    case (int)HAZARDTYPE.HZ_BOX_SPAWN:
                        BoxSpawnCount++;
                        hazard = new BoxSpawn();
                        hazard.Name = "Box Spawn" + BoxSpawnCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_BOX_SPAWN;
                        break;
                    case (int)HAZARDTYPE.HZ_DOOR:
                        DoorCount++;
                        hazard = new Door();
                        hazard.Name = "Door" + DoorCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_DOOR;
                        break;
                    case (int)HAZARDTYPE.HZ_DOOR_BUTTON:
                        DoorButtonCount++;
                        hazard = new DoorButton();
                        hazard.Name = "Door Button" + DoorButtonCount.ToString();
                        hazard.Type = (int)HAZARDTYPE.HZ_DOOR_BUTTON;
                        break;
                    default:
                        continue;
                }

                hazard.VectorPos = i;
                hazard.Position = pos;

                RadioButton button = new RadioButton();
                button.Text = hazard.Name;
                button.Location = hazard.Position;
                Graphics gp = CreateGraphics();
                button.Width = (int)(gp.MeasureString(button.Text, button.Font).Width + 20);
                button.CheckedChanged += new System.EventHandler(this.Radio_CheckedChanged);

                hazard.Button = button;

                level.Hazards.Add(hazard);
                DisplayPanel.Controls.Add(button);
            }

           
        }

        private void LoadNewLightData(Level lvl)
        {
            int LightCount = _GetNumLights();

            for (int LightIndex = 0; LightIndex < LightCount; LightIndex++)
            {

                Light light = null;
                int Type = _GetLightType(LightIndex);
                switch (Type)
                {
                    case (int)LIGHTTYPE.LT_DIRECTIONAL:
                        {
                            light = new DirectionalLight();
                        }
                        break;
                    case (int)LIGHTTYPE.LT_POINT:
                        {
                            light = new PointLight();
                        }
                        break;
                    case (int)LIGHTTYPE.LT_SPOT:
                        {
                            light = new SpotLight();
                        }
                        break;
                }

               BASELIGHT baselight = _GetLightBase(LightIndex);
                light.Type = Type;

                light.Ambient = Color.FromArgb((int)(baselight.AmbientColorW), (int)(baselight.AmbientColorX), (int)(baselight.AmbientColorY), (int)(baselight.AmbientColorZ));
                light.Diffuse = Color.FromArgb((int)(baselight.DiffuseColorW), (int)(baselight.DiffuseColorX), (int)(baselight.DiffuseColorY), (int)(baselight.DiffuseColorZ));
                light.Specular = Color.FromArgb((int)(baselight.SpecularColorW), (int)(baselight.SpecularColorX), (int)(baselight.SpecularColorY), (int)(baselight.SpecularColorZ));

                light.SpecIntensity = baselight.SpecularIntensity;
                light.Specpower = baselight.SpecularPower;

                switch (light.Type)
                {
                    case (int)LIGHTTYPE.LT_DIRECTIONAL:
                        {
                            ((DirectionalLight)light).Direction = _GetLightDir(LightIndex);
                           // _AddDirectionalLight(baselight, ((DirectionalLight)light).Direction);
                        }
                        break;
                    case (int)LIGHTTYPE.LT_POINT:
                        {
                            ((PointLight)light).Atten = _GetLightAtten(LightIndex);
                            ((PointLight)light).Position = _GetLightPos(LightIndex);
                            ((PointLight)light).Radius = _GetLightRadius(LightIndex);

                            //_AddPointLight(baselight, ((PointLight)light).Position, ((PointLight)light).Atten, ((PointLight)light).Radius);
                        }
                        break;
                    case (int)LIGHTTYPE.LT_SPOT:
                        {
                            ((SpotLight)light).Atten = _GetLightAtten(LightIndex);
                            ((SpotLight)light).Position = _GetLightPos(LightIndex);
                            ((SpotLight)light).Radius = _GetLightRadius(LightIndex);
                            ((SpotLight)light).Cutoff = _GetLightCutOff(LightIndex);
                            ((SpotLight)light).Direction = _GetLightDir(LightIndex);
                            ((SpotLight)light).Expo = _GetLightExpo(LightIndex);
                            //_AddSpotLight(baselight, ((SpotLight)light).Position, ((SpotLight)light).Direction, ((SpotLight)light).Atten, ((SpotLight)light).Radius, ((SpotLight)light).Cutoff, ((SpotLight)light).Expo);
                        }
                        break;
                }

                light.VecPos = LightIndex;

                LevelLights.Items.Add(light);
                lvl.Lights.Add(light);
                LevelLights.SelectedIndex = light.VecPos;
            }
        }

        private void LoadLevelData(Level lvl, BinaryReader binRead, byte[] ReadBuffer)
        {
            ClearBuffer(ReadBuffer);
            binRead.Read(ReadBuffer, 0, sizeof(int));
            int NameLength = BitConverter.ToInt32(ReadBuffer, 0);

            ClearBuffer(ReadBuffer);
            string LevelName = "";
            // Read(ReadBuffer, 0, sizeof(char) * NextLevelLength);
            if (NameLength > 0)
            {
                for (int index = 0; index < NameLength; index++)
                {
                    LevelName += binRead.ReadChar();
                }
                //NextLevel = BitConverter.ToString(ReadBuffer, 0);
            }

            lvl.Name = LevelName;

            CheckPoint checkpoint = new CheckPoint();
            VEC3F Pos;

            Pos.x = 0.0f;
            Pos.y = 0.0f;
            Pos.z = 0.0f;

            ClearBuffer(ReadBuffer);
            binRead.Read(ReadBuffer, 0, sizeof(double));
            Pos.x = (float)BitConverter.ToDouble(ReadBuffer, 0);

            ClearBuffer(ReadBuffer);
            binRead.Read(ReadBuffer, 0, sizeof(double));
            Pos.y = (float)BitConverter.ToDouble(ReadBuffer, 0);

            ClearBuffer(ReadBuffer);
            binRead.Read(ReadBuffer, 0, sizeof(double));
            Pos.z = (float)BitConverter.ToDouble(ReadBuffer, 0);

            checkpoint.Position = Pos;

            ClearBuffer(ReadBuffer);
            binRead.Read(ReadBuffer, 0, sizeof(int));
            int NextLevelLength = BitConverter.ToInt32(ReadBuffer, 0);

            ClearBuffer(ReadBuffer);
            string NextLevel = "";
            // Read(ReadBuffer, 0, sizeof(char) * NextLevelLength);
            if (NextLevelLength > 0)
            {
                for (int index = 0; index < NextLevelLength; index++)
                {
                    NextLevel += binRead.ReadChar();
                }
                //NextLevel = BitConverter.ToString(ReadBuffer, 0);
            }

            checkpoint.NextLevelName = NextLevel;

            lvl.LevelCheckPoint = checkpoint;

            VEC3F max, min;
            try
            {
                max.x = binRead.ReadSingle();
                max.y = binRead.ReadSingle();
                max.z = binRead.ReadSingle();

                min.x = binRead.ReadSingle();
                min.y = binRead.ReadSingle();
                min.z = binRead.ReadSingle();
            }
            catch
            {
                max.x = 0.0f;
                max.y = 0.0f;
                max.z = 0.0f;

                min.x = 0.0f;
                min.y = 0.0f;
                min.z = 0.0f;
            }

            lvl.MaxKdTree = max;
            lvl.MinKdTree = min;
        }

        private void LoadLevelObjectData(Level level, int NumObjects, BinaryReader binRead, byte[] ReadBuffer)
        {
            int LinkObjectCount = _GetNumObjects();
            for (int ObjectIndex = 0; ObjectIndex < _GetNumObjects(); )
            {
                _RemoveObject(ObjectIndex);
            }

            for (int ObjectIndex = 0; ObjectIndex < NumObjects; ObjectIndex++)
            {
                LevelObject lvlObject = new LevelObject();

                lvlObject.ArrayPos = ObjectIndex;

                ClearBuffer(ReadBuffer);
                binRead.Read(ReadBuffer, 0, sizeof(int));
                lvlObject.Type = BitConverter.ToInt32(ReadBuffer, 0);

                ClearBuffer(ReadBuffer);
                VEC3F pos;
                binRead.Read(ReadBuffer, 0, sizeof(double));
                pos.x = (float)BitConverter.ToDouble(ReadBuffer, 0);

                ClearBuffer(ReadBuffer);
                binRead.Read(ReadBuffer, 0, sizeof(double));
                pos.y = (float)BitConverter.ToDouble(ReadBuffer, 0);

                ClearBuffer(ReadBuffer);
                binRead.Read(ReadBuffer, 0, sizeof(double));
                pos.z = (float)BitConverter.ToDouble(ReadBuffer, 0);

                lvlObject.Pos = pos;

                level.Objects.Add(lvlObject);
                ObjPositionList.Items.Add(lvlObject);

                _AddObject(lvlObject.Type, lvlObject.Pos);
            }
        }

        private void LoadLevelHazardData(Level level, int NumHazards, BinaryReader binRead, byte[] ReadBuffer)
        {
            int SpawnPipeCount = 0;
            int SteamPipeCount = 0;
            int SteamIntakeCount = 0;
            int CrusherCount = 0;
            int GooPitCount = 0;
            int BridgeCount = 0;
            int BreakableWallCount = 0;
            int OutletCount = 0;
            int WaterPoolCount = 0;
            int BoxButtonCount = 0;
            int BoxSpawnCount = 0;
            int DoorCount = 0;
            int DoorButtonCount = 0;

            for (int HazardIndex = 0; HazardIndex < NumHazards; HazardIndex++)
            {
                VEC3F hazardPos = _GetHazard(HazardIndex);
                Point pos = new Point();
                pos.X = (int)hazardPos.x;
                pos.Y = (int)hazardPos.y;

                Hazards hazard = null;

                ClearBuffer(ReadBuffer);
                binRead.Read(ReadBuffer, 0, sizeof(int));
                int type = BitConverter.ToInt32(ReadBuffer, 0);

                ClearBuffer(ReadBuffer);
                binRead.Read(ReadBuffer, 0, sizeof(int));
                int channel = BitConverter.ToInt32(ReadBuffer, 0);

                switch (type)
                {
                    case (int)HAZARDTYPE.HZ_STEAM_PIPE:
                        {
                            SteamPipeCount++;
                            hazard = new SteamPipe();
                            hazard.Name = "SteamOut" + SteamPipeCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_STEAM_PIPE;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(bool));
                            ((SteamPipe)hazard).Constant = BitConverter.ToBoolean(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            ((SteamPipe)hazard).SteamLength = BitConverter.ToInt32(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(double));
                            ((SteamPipe)hazard).Duration = (float)BitConverter.ToDouble(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(double));
                            ((SteamPipe)hazard).Frequency = (float)BitConverter.ToDouble(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            ((SteamPipe)hazard).Direction = BitConverter.ToInt32(ReadBuffer, 0);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_STEAM_INTAKE:
                        {
                            SteamIntakeCount++;
                            hazard = new SteamIntake();
                            hazard.Name = "SteamIntake" + SteamIntakeCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_STEAM_INTAKE;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(bool));
                            ((SteamIntake)hazard).Corkable = BitConverter.ToBoolean(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            ((SteamIntake)hazard).Direction = BitConverter.ToInt32(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            int StringLength = BitConverter.ToInt32(ReadBuffer, 0);

                            string FlowName = "";

                            FlowName = new string(binRead.ReadChars(StringLength));
                            
                            ((SteamIntake)hazard).FlowName = FlowName;
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_SPAWN_PIPE:
                        {
                            SpawnPipeCount++;
                            hazard = new SpawnPipe();
                            hazard.Name = "Spawn" + SpawnPipeCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_SPAWN_PIPE;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(bool));
                            ((SpawnPipe)hazard).Corkable = BitConverter.ToBoolean(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            ((SpawnPipe)hazard).MaxSpawns = BitConverter.ToInt32(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(double));
                            ((SpawnPipe)hazard).Frequency = (float)BitConverter.ToDouble(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            ((SpawnPipe)hazard).Direction = BitConverter.ToInt32(ReadBuffer, 0);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_CRUSHER:
                        {
                            CrusherCount++;
                            hazard = new Crusher();
                            hazard.Name = "Crusher" + CrusherCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_CRUSHER;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            ((Crusher)hazard).Length = BitConverter.ToInt32(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(double));
                            ((Crusher)hazard).Duration = (float)BitConverter.ToDouble(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(double));
                            ((Crusher)hazard).Frequency = (float)BitConverter.ToDouble(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            ((Crusher)hazard).Direction = BitConverter.ToInt32(ReadBuffer, 0);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_CRUSHER_BASE:
                        {
                            hazard = new CrusherBase();
                            hazard.Name = "CrusherBase";
                            hazard.Type = (int)HAZARDTYPE.HZ_CRUSHER_BASE;
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_GOO_PIT:
                        {
                            GooPitCount++;
                            hazard = new GooPit();
                            hazard.Name = "GooPit" + GooPitCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_GOO_PIT;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(double));
                            ((GooPit)hazard).Damage = (float)BitConverter.ToDouble(ReadBuffer, 0);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_BRIDGE:
                        {
                            BridgeCount++;
                            hazard = new Bridge();
                            hazard.Name = "Bridge" + BridgeCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_BRIDGE;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            ((Bridge)hazard).Quadrant = BitConverter.ToInt32(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(bool));
                            ((Bridge)hazard).RotateClockwise = BitConverter.ToBoolean(ReadBuffer, 0);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_OUTLET:
                        {
                            OutletCount++;
                            hazard = new OutLet();
                            hazard.Name = "OutLet" + OutletCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_OUTLET;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            ((OutLet)hazard).Direction = BitConverter.ToInt32(ReadBuffer, 0);
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_BREAKABLE_WALL:
                        {
                            BreakableWallCount++;
                            hazard = new BreakableWall();
                            hazard.Name = "Breakable Wall" + BreakableWallCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_BREAKABLE_WALL;
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_WATER_POOL:
                        {
                            WaterPoolCount++;
                            hazard = new WaterPool();
                            hazard.Name = "WaterPool" + WaterPoolCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_WATER_POOL;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(double));
                            ((WaterPool)hazard).HealAmount = (float)BitConverter.ToDouble(ReadBuffer, 0);

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            int StringLength = BitConverter.ToInt32(ReadBuffer, 0);

                            string MeshName = "";

                            MeshName = new string(binRead.ReadChars(StringLength));

                            ((WaterPool)hazard).MeshName = MeshName;
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_BOX_BUTTON:
                        {
                            BoxButtonCount++;
                            hazard = new BoxButton();
                            hazard.Name = "Box Button" + BoxButtonCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_BOX_BUTTON;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            int StringLength = BitConverter.ToInt32(ReadBuffer, 0);

                            string WireName = "";

                            WireName = new string(binRead.ReadChars(StringLength));

                            ((BoxButton)hazard).WireName = WireName;
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_BOX_SPAWN:
                        {
                            BoxSpawnCount++;
                            hazard = new BoxSpawn();
                            hazard.Name = "Box Spawn" + BoxSpawnCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_BOX_SPAWN;
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_DOOR:
                        {
                            DoorCount++;
                            hazard = new Door();
                            hazard.Name = "Door" + DoorCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_DOOR;
                        }
                        break;
                    case (int)HAZARDTYPE.HZ_DOOR_BUTTON:
                        {
                            DoorButtonCount++;
                            hazard = new DoorButton();
                            hazard.Name = "Door Button" + DoorButtonCount.ToString();
                            hazard.Type = (int)HAZARDTYPE.HZ_DOOR_BUTTON;

                            ClearBuffer(ReadBuffer);
                            binRead.Read(ReadBuffer, 0, sizeof(int));
                            int StringLength = BitConverter.ToInt32(ReadBuffer, 0);

                            string WireName = "";

                            WireName = new string(binRead.ReadChars(StringLength));

                            if (StringLength > 0)
                            {
                                //WireName = new string(binRead.ReadChars(StringLength));
                            }
                            ((DoorButton)hazard).WireName = WireName;
                        }
                        break;
                }

                hazard.Channel = channel;
                hazard.VectorPos = HazardIndex;
                hazard.Position = pos;

                RadioButton button = new RadioButton();
                button.Text = hazard.Name;
                button.Location = hazard.Position;
                Graphics gp = CreateGraphics();
                button.Width = (int)(gp.MeasureString(button.Text, button.Font).Width + 20);
                button.CheckedChanged += new System.EventHandler(this.Radio_CheckedChanged);

                hazard.Button = button;

                level.Hazards.Add(hazard);
                DisplayPanel.Controls.Add(button);

            }
        }

        private void LoadLevelLightData(Level lvl, int NumLights, BinaryReader binRead, byte[] ReadBuffer)
        {
            for (int LightIndex = 0; LightIndex < _GetNumLights(); )
            {
                _RemoveLight(LightIndex);
            }

            for (int LightIndex = 0; LightIndex < NumLights; LightIndex++)
            {
                ClearBuffer(ReadBuffer);
                binRead.Read(ReadBuffer, 0, sizeof (int));
                int Type = BitConverter.ToInt32(ReadBuffer, 0);

                Light light = null;

                switch (Type)
                {
                    case (int)LIGHTTYPE.LT_DIRECTIONAL:
                        {
                            light = new DirectionalLight();
                        }
                        break;
                    case (int)LIGHTTYPE.LT_POINT:
                        {
                            light = new PointLight();
                        }
                        break;
                    case (int)LIGHTTYPE.LT_SPOT:
                        {
                            light = new SpotLight();
                        }
                        break;
                }

                light.Type = Type;

                float Alpha, Red, Green, Blue;

                
                ClearBuffer(ReadBuffer);
                Alpha = binRead.ReadSingle();
                Red = binRead.ReadSingle();
                Green = binRead.ReadSingle();
                Blue = binRead.ReadSingle();
                
                light.Ambient = Color.FromArgb((int)(Alpha*255), (int)(Red*255), (int)(Green*255), (int)(Blue*255));

                Alpha = binRead.ReadSingle();
                Red = binRead.ReadSingle();
                Green = binRead.ReadSingle();
                Blue = binRead.ReadSingle();

                light.Diffuse = Color.FromArgb((int)(Alpha * 255), (int)(Red * 255), (int)(Green * 255), (int)(Blue * 255));

                Alpha = binRead.ReadSingle();
                Red = binRead.ReadSingle();
                Green = binRead.ReadSingle();
                Blue = binRead.ReadSingle();

                light.Specular = Color.FromArgb((int)(Alpha * 255), (int)(Red * 255), (int)(Green * 255), (int)(Blue * 255));

                light.SpecIntensity = binRead.ReadSingle();
                light.Specpower = binRead.ReadSingle();


                BASELIGHT baselight;
                baselight.DiffuseColorX = light.Diffuse.R;
                baselight.DiffuseColorY = light.Diffuse.G;
                baselight.DiffuseColorZ = light.Diffuse.B;
                baselight.DiffuseColorW = light.Diffuse.A;

                baselight.AmbientColorX = light.Ambient.R;
                baselight.AmbientColorY = light.Ambient.G;
                baselight.AmbientColorZ = light.Ambient.B;
                baselight.AmbientColorW = light.Ambient.A;

                baselight.SpecularColorX = light.Specular.R;
                baselight.SpecularColorY = light.Specular.G;
                baselight.SpecularColorZ = light.Specular.B;
                baselight.SpecularColorW = light.Specular.A;

                baselight.SpecularIntensity = light.SpecIntensity;
                baselight.SpecularPower = light.Specpower;

                switch (light.Type)
                {
                    case (int)LIGHTTYPE.LT_DIRECTIONAL:
                        {
                            VEC3F dir;
                            dir.x = binRead.ReadSingle();
                            dir.y = binRead.ReadSingle();
                            dir.z = binRead.ReadSingle();
                            
                            ((DirectionalLight)light).Direction = dir;

                            _AddDirectionalLight(baselight, ((DirectionalLight)light).Direction);
                        }
                        break;
                    case (int)LIGHTTYPE.LT_POINT:
                        {
                            VEC3F atten;
                            atten.x = binRead.ReadSingle();
                            atten.y = binRead.ReadSingle();
                            atten.z = binRead.ReadSingle();

                            ((PointLight)light).Atten = atten;

                            VEC3F pos;
                            pos.x = binRead.ReadSingle();
                            pos.y = binRead.ReadSingle();
                            pos.z = binRead.ReadSingle();

                            ((PointLight)light).Position = pos;

                            ((PointLight)light).Radius = binRead.ReadSingle();

                            _AddPointLight(baselight, ((PointLight)light).Position, ((PointLight)light).Atten, ((PointLight)light).Radius);
                        }
                        break;
                    case (int)LIGHTTYPE.LT_SPOT:
                        {
                            VEC3F atten;
                            atten.x = binRead.ReadSingle();
                            atten.y = binRead.ReadSingle();
                            atten.z = binRead.ReadSingle();

                            ((SpotLight)light).Atten = atten;

                            VEC3F pos;
                            pos.x = binRead.ReadSingle();
                            pos.y = binRead.ReadSingle();
                            pos.z = binRead.ReadSingle();

                            ((SpotLight)light).Position = pos;

                            ((SpotLight)light).Radius = binRead.ReadSingle();
                            ((SpotLight)light).Cutoff = binRead.ReadSingle();

                            VEC3F dir;
                            dir.x = binRead.ReadSingle();
                            dir.y = binRead.ReadSingle();
                            dir.z = binRead.ReadSingle();

                            ((SpotLight)light).Direction = dir;

                            ((SpotLight)light).Expo = binRead.ReadSingle();

                            _AddSpotLight(baselight, ((SpotLight)light).Position, ((SpotLight)light).Direction, ((SpotLight)light).Atten, ((SpotLight)light).Radius, ((SpotLight)light).Cutoff, ((SpotLight)light).Expo);
                        }
                        break;
                }

                light.VecPos = LevelLights.Items.Count;

                LevelLights.Items.Add(light);
                lvl.Lights.Add(light);
            }
        }

        private void ClearBuffer(byte[] Buffer)
        {
            for (int index = 0; index < Buffer.Length; ++index)
            {
                Buffer[index] = 0;
            }
        }
        #endregion


        #region HandleKeyPresses
        // keep the split container from moving
        private void splitContainer1_SplitterMoving(object sender, SplitterCancelEventArgs e)
        {
            splitContainer1.SplitterDistance = SplitDistanceConst;
        }

        // keep the split container from moving
        private void splitContainer1_SplitterMoved(object sender, SplitterEventArgs e)
        {
            splitContainer1.SplitterDistance = SplitDistanceConst;
        }

        // keep the user from pressing keys in the level list box
        private void LevelList_KeyDown(object sender, KeyEventArgs e)
        {
           e.Handled = true;
            e.SuppressKeyPress = true;
        }

        // keep the user from pressing keys in the level list box
        private void LevelList_KeyPress(object sender, KeyPressEventArgs e)
        {
           e.Handled = true;
        }

        // keep the user from pressing keys in the object type combo box
        private void ObjTypeBox_KeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = true;
        }

        // keep the user from pressing keys in the object type combo box
        private void ObjTypeBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
        }

        // keep the user from pressing keys in the Light type combo box
        private void LightType_KeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = true;
        }

        // keep the user from pressing keys in the Light type combo box
        private void LightType_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
        }
        #endregion

       

        
    }
}
