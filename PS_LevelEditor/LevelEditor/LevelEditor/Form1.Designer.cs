namespace LevelEditor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.MainTabPanel = new System.Windows.Forms.TabControl();
            this.LevelsTab = new System.Windows.Forms.TabPage();
            this.OpenAll = new System.Windows.Forms.Button();
            this.Min = new System.Windows.Forms.GroupBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.KDTreeMinZ = new System.Windows.Forms.NumericUpDown();
            this.KDTreeMinY = new System.Windows.Forms.NumericUpDown();
            this.label12 = new System.Windows.Forms.Label();
            this.KDTreeMinX = new System.Windows.Forms.NumericUpDown();
            this.KDTreeMax = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.KDTreeMaxZ = new System.Windows.Forms.NumericUpDown();
            this.KDTreeMaxY = new System.Windows.Forms.NumericUpDown();
            this.label9 = new System.Windows.Forms.Label();
            this.KDTreeMaxX = new System.Windows.Forms.NumericUpDown();
            this.CheckPointInfo = new System.Windows.Forms.GroupBox();
            this.CheckPointPosZLabel = new System.Windows.Forms.Label();
            this.CheckPointPosYLabel = new System.Windows.Forms.Label();
            this.CheckPointPosZ = new System.Windows.Forms.NumericUpDown();
            this.CheckPointPosY = new System.Windows.Forms.NumericUpDown();
            this.CheckPointPosXLabel = new System.Windows.Forms.Label();
            this.CheckPointPosX = new System.Windows.Forms.NumericUpDown();
            this.SaveAll = new System.Windows.Forms.Button();
            this.LevelOpen = new System.Windows.Forms.Button();
            this.LevelDown = new System.Windows.Forms.Button();
            this.LevelDelete = new System.Windows.Forms.Button();
            this.LevelUp = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.LevelList = new System.Windows.Forms.ListBox();
            this.ObjPositionsTab = new System.Windows.Forms.TabPage();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.ObjPosZ = new System.Windows.Forms.NumericUpDown();
            this.ObjPosY = new System.Windows.Forms.NumericUpDown();
            this.ObjPosX = new System.Windows.Forms.NumericUpDown();
            this.ObjTypeBox = new System.Windows.Forms.ComboBox();
            this.RemoveObject = new System.Windows.Forms.Button();
            this.AddObject = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.ObjPositionList = new System.Windows.Forms.ListBox();
            this.HazardInfoTab = new System.Windows.Forms.TabPage();
            this.ObjsChannelsLabel = new System.Windows.Forms.Label();
            this.ChannelList = new System.Windows.Forms.ListBox();
            this.WaterPoolChanLabel = new System.Windows.Forms.Label();
            this.HazardChannel = new System.Windows.Forms.NumericUpDown();
            this.LightInfoTab = new System.Windows.Forms.TabPage();
            this.RemoveLight = new System.Windows.Forms.Button();
            this.AddLight = new System.Windows.Forms.Button();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.SpecularIntensity = new System.Windows.Forms.NumericUpDown();
            this.SpecularPower = new System.Windows.Forms.NumericUpDown();
            this.SpecularButton = new System.Windows.Forms.Button();
            this.AmbientButton = new System.Windows.Forms.Button();
            this.DiffuseButton = new System.Windows.Forms.Button();
            this.DirLightAtt = new System.Windows.Forms.GroupBox();
            this.DirectionalDirZ = new System.Windows.Forms.NumericUpDown();
            this.DirectionalDirY = new System.Windows.Forms.NumericUpDown();
            this.DirectionalDirX = new System.Windows.Forms.NumericUpDown();
            this.label19 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.LightType = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.LevelLights = new System.Windows.Forms.ListBox();
            this.DisplayPanel = new System.Windows.Forms.Panel();
            this.SpotLightAttr = new System.Windows.Forms.GroupBox();
            this.label32 = new System.Windows.Forms.Label();
            this.label31 = new System.Windows.Forms.Label();
            this.SpotRadius = new System.Windows.Forms.NumericUpDown();
            this.label33 = new System.Windows.Forms.Label();
            this.SpotAttenZ = new System.Windows.Forms.NumericUpDown();
            this.SpotAttenY = new System.Windows.Forms.NumericUpDown();
            this.SpotAttenX = new System.Windows.Forms.NumericUpDown();
            this.label30 = new System.Windows.Forms.Label();
            this.label27 = new System.Windows.Forms.Label();
            this.SpotExponent = new System.Windows.Forms.NumericUpDown();
            this.SpotExponentLabel = new System.Windows.Forms.Label();
            this.label28 = new System.Windows.Forms.Label();
            this.SpotCutOff = new System.Windows.Forms.NumericUpDown();
            this.SpotCutOffLabel = new System.Windows.Forms.Label();
            this.label29 = new System.Windows.Forms.Label();
            this.SpotDirZ = new System.Windows.Forms.NumericUpDown();
            this.SpotPosZ = new System.Windows.Forms.NumericUpDown();
            this.SpotDirY = new System.Windows.Forms.NumericUpDown();
            this.SpotDirX = new System.Windows.Forms.NumericUpDown();
            this.SpotPosY = new System.Windows.Forms.NumericUpDown();
            this.SpotDirZLabel = new System.Windows.Forms.Label();
            this.SpotDirYLabel = new System.Windows.Forms.Label();
            this.SpotPosX = new System.Windows.Forms.NumericUpDown();
            this.SpotDirXLabel = new System.Windows.Forms.Label();
            this.PointLightAttr = new System.Windows.Forms.GroupBox();
            this.label24 = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.PointLAttenZ = new System.Windows.Forms.NumericUpDown();
            this.PointLAttenY = new System.Windows.Forms.NumericUpDown();
            this.PointLAttenX = new System.Windows.Forms.NumericUpDown();
            this.label23 = new System.Windows.Forms.Label();
            this.PointLRadius = new System.Windows.Forms.NumericUpDown();
            this.label22 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.PointLPosZ = new System.Windows.Forms.NumericUpDown();
            this.PointLPosY = new System.Windows.Forms.NumericUpDown();
            this.PointLPosX = new System.Windows.Forms.NumericUpDown();
            this.BoxSpawnInfo = new System.Windows.Forms.GroupBox();
            this.DoorInfo = new System.Windows.Forms.GroupBox();
            this.DoorButtonInfo = new System.Windows.Forms.GroupBox();
            this.label35 = new System.Windows.Forms.Label();
            this.DoorButtonWireName = new System.Windows.Forms.TextBox();
            this.BoxButtonInfo = new System.Windows.Forms.GroupBox();
            this.label34 = new System.Windows.Forms.Label();
            this.BoxButtonWireName = new System.Windows.Forms.TextBox();
            this.GooInfo = new System.Windows.Forms.GroupBox();
            this.ConnectAllPits = new System.Windows.Forms.CheckBox();
            this.GooDamageLabel = new System.Windows.Forms.Label();
            this.GooDamage = new System.Windows.Forms.NumericUpDown();
            this.CrusherInfo = new System.Windows.Forms.GroupBox();
            this.CrusherDirLabel = new System.Windows.Forms.Label();
            this.CrusherDir = new System.Windows.Forms.ComboBox();
            this.CrusherDurationLable = new System.Windows.Forms.Label();
            this.CrusherDuration = new System.Windows.Forms.NumericUpDown();
            this.CrusherFreq = new System.Windows.Forms.NumericUpDown();
            this.CrusherFreqLable = new System.Windows.Forms.Label();
            this.CrusherLengthLable = new System.Windows.Forms.Label();
            this.CrusherLength = new System.Windows.Forms.NumericUpDown();
            this.BridgeInfo = new System.Windows.Forms.GroupBox();
            this.BridgeRotate = new System.Windows.Forms.CheckBox();
            this.BridgeQuadLabel = new System.Windows.Forms.Label();
            this.BridgeQuadrant = new System.Windows.Forms.ComboBox();
            this.OutletInfo = new System.Windows.Forms.GroupBox();
            this.OutletDirLabel = new System.Windows.Forms.Label();
            this.OutletDir = new System.Windows.Forms.ComboBox();
            this.SpawnInfo = new System.Windows.Forms.GroupBox();
            this.SpawnDirLable = new System.Windows.Forms.Label();
            this.SpawnDir = new System.Windows.Forms.ComboBox();
            this.SpawnCorkable = new System.Windows.Forms.CheckBox();
            this.SpawnLimitLable = new System.Windows.Forms.Label();
            this.SpawnLimit = new System.Windows.Forms.NumericUpDown();
            this.SpawnFreqLable = new System.Windows.Forms.Label();
            this.SpawnFreq = new System.Windows.Forms.NumericUpDown();
            this.SteamInfo = new System.Windows.Forms.GroupBox();
            this.SteamDirLabel = new System.Windows.Forms.Label();
            this.SteamDir = new System.Windows.Forms.ComboBox();
            this.SteamFreqLable = new System.Windows.Forms.Label();
            this.SteamFreq = new System.Windows.Forms.NumericUpDown();
            this.SteamDurationLable = new System.Windows.Forms.Label();
            this.SteamDuration = new System.Windows.Forms.NumericUpDown();
            this.SteamLengthLable = new System.Windows.Forms.Label();
            this.SteamLength = new System.Windows.Forms.NumericUpDown();
            this.SteamConstant = new System.Windows.Forms.CheckBox();
            this.WaterPoolInfo = new System.Windows.Forms.GroupBox();
            this.PoolMeshLabel = new System.Windows.Forms.Label();
            this.PoolMeshName = new System.Windows.Forms.TextBox();
            this.WaterPoolHealLabel = new System.Windows.Forms.Label();
            this.WaterPoolHeal = new System.Windows.Forms.NumericUpDown();
            this.IntakeInfo = new System.Windows.Forms.GroupBox();
            this.FlowPipeLabel = new System.Windows.Forms.Label();
            this.FlowPipeName = new System.Windows.Forms.TextBox();
            this.IntakeDirLabel = new System.Windows.Forms.Label();
            this.IntakeDir = new System.Windows.Forms.ComboBox();
            this.IntakeCorkable = new System.Windows.Forms.CheckBox();
            this.BreakableWallInfo = new System.Windows.Forms.GroupBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.MainTabPanel.SuspendLayout();
            this.LevelsTab.SuspendLayout();
            this.Min.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMinZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMinY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMinX)).BeginInit();
            this.KDTreeMax.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMaxZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMaxY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMaxX)).BeginInit();
            this.CheckPointInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.CheckPointPosZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.CheckPointPosY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.CheckPointPosX)).BeginInit();
            this.ObjPositionsTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ObjPosZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ObjPosY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ObjPosX)).BeginInit();
            this.HazardInfoTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.HazardChannel)).BeginInit();
            this.LightInfoTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SpecularIntensity)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpecularPower)).BeginInit();
            this.DirLightAtt.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DirectionalDirZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.DirectionalDirY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.DirectionalDirX)).BeginInit();
            this.DisplayPanel.SuspendLayout();
            this.SpotLightAttr.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SpotRadius)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotAttenZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotAttenY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotAttenX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotExponent)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotCutOff)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotDirZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotPosZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotDirY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotDirX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotPosY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotPosX)).BeginInit();
            this.PointLightAttr.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PointLAttenZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLAttenY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLAttenX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLRadius)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLPosZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLPosY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLPosX)).BeginInit();
            this.DoorButtonInfo.SuspendLayout();
            this.BoxButtonInfo.SuspendLayout();
            this.GooInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.GooDamage)).BeginInit();
            this.CrusherInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.CrusherDuration)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.CrusherFreq)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.CrusherLength)).BeginInit();
            this.BridgeInfo.SuspendLayout();
            this.OutletInfo.SuspendLayout();
            this.SpawnInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SpawnLimit)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpawnFreq)).BeginInit();
            this.SteamInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SteamFreq)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SteamDuration)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SteamLength)).BeginInit();
            this.WaterPoolInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.WaterPoolHeal)).BeginInit();
            this.IntakeInfo.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.MainTabPanel);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.DisplayPanel);
            this.splitContainer1.Size = new System.Drawing.Size(1214, 892);
            this.splitContainer1.SplitterDistance = 253;
            this.splitContainer1.TabIndex = 0;
            this.splitContainer1.SplitterMoving += new System.Windows.Forms.SplitterCancelEventHandler(this.splitContainer1_SplitterMoving);
            this.splitContainer1.SplitterMoved += new System.Windows.Forms.SplitterEventHandler(this.splitContainer1_SplitterMoved);
            // 
            // MainTabPanel
            // 
            this.MainTabPanel.Controls.Add(this.LevelsTab);
            this.MainTabPanel.Controls.Add(this.ObjPositionsTab);
            this.MainTabPanel.Controls.Add(this.HazardInfoTab);
            this.MainTabPanel.Controls.Add(this.LightInfoTab);
            this.MainTabPanel.Location = new System.Drawing.Point(3, 3);
            this.MainTabPanel.Name = "MainTabPanel";
            this.MainTabPanel.SelectedIndex = 0;
            this.MainTabPanel.Size = new System.Drawing.Size(248, 864);
            this.MainTabPanel.TabIndex = 0;
            this.MainTabPanel.SelectedIndexChanged += new System.EventHandler(this.MainTabPanel_SelectedIndexChanged);
            // 
            // LevelsTab
            // 
            this.LevelsTab.Controls.Add(this.OpenAll);
            this.LevelsTab.Controls.Add(this.Min);
            this.LevelsTab.Controls.Add(this.KDTreeMax);
            this.LevelsTab.Controls.Add(this.CheckPointInfo);
            this.LevelsTab.Controls.Add(this.SaveAll);
            this.LevelsTab.Controls.Add(this.LevelOpen);
            this.LevelsTab.Controls.Add(this.LevelDown);
            this.LevelsTab.Controls.Add(this.LevelDelete);
            this.LevelsTab.Controls.Add(this.LevelUp);
            this.LevelsTab.Controls.Add(this.label1);
            this.LevelsTab.Controls.Add(this.LevelList);
            this.LevelsTab.Location = new System.Drawing.Point(4, 22);
            this.LevelsTab.Name = "LevelsTab";
            this.LevelsTab.Padding = new System.Windows.Forms.Padding(3);
            this.LevelsTab.Size = new System.Drawing.Size(240, 838);
            this.LevelsTab.TabIndex = 1;
            this.LevelsTab.Text = "Levels";
            this.LevelsTab.UseVisualStyleBackColor = true;
            // 
            // OpenAll
            // 
            this.OpenAll.Location = new System.Drawing.Point(39, 297);
            this.OpenAll.Name = "OpenAll";
            this.OpenAll.Size = new System.Drawing.Size(75, 23);
            this.OpenAll.TabIndex = 34;
            this.OpenAll.Text = "Open All";
            this.OpenAll.UseVisualStyleBackColor = true;
            this.OpenAll.Click += new System.EventHandler(this.OpenAll_Click);
            // 
            // Min
            // 
            this.Min.Controls.Add(this.label10);
            this.Min.Controls.Add(this.label11);
            this.Min.Controls.Add(this.KDTreeMinZ);
            this.Min.Controls.Add(this.KDTreeMinY);
            this.Min.Controls.Add(this.label12);
            this.Min.Controls.Add(this.KDTreeMinX);
            this.Min.Location = new System.Drawing.Point(6, 654);
            this.Min.Name = "Min";
            this.Min.Size = new System.Drawing.Size(197, 132);
            this.Min.TabIndex = 33;
            this.Min.TabStop = false;
            this.Min.Text = "Min";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(19, 100);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(35, 13);
            this.label10.TabIndex = 31;
            this.label10.Text = "Pos Z";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(19, 68);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(35, 13);
            this.label11.TabIndex = 30;
            this.label11.Text = "Pos Y";
            // 
            // KDTreeMinZ
            // 
            this.KDTreeMinZ.DecimalPlaces = 3;
            this.KDTreeMinZ.Location = new System.Drawing.Point(71, 98);
            this.KDTreeMinZ.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.KDTreeMinZ.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.KDTreeMinZ.Name = "KDTreeMinZ";
            this.KDTreeMinZ.Size = new System.Drawing.Size(120, 20);
            this.KDTreeMinZ.TabIndex = 29;
            this.KDTreeMinZ.ValueChanged += new System.EventHandler(this.KDTreeMinZ_ValueChanged);
            // 
            // KDTreeMinY
            // 
            this.KDTreeMinY.DecimalPlaces = 3;
            this.KDTreeMinY.Location = new System.Drawing.Point(71, 66);
            this.KDTreeMinY.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.KDTreeMinY.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.KDTreeMinY.Name = "KDTreeMinY";
            this.KDTreeMinY.Size = new System.Drawing.Size(120, 20);
            this.KDTreeMinY.TabIndex = 28;
            this.KDTreeMinY.ValueChanged += new System.EventHandler(this.KDTreeMinY_ValueChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(19, 36);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(35, 13);
            this.label12.TabIndex = 27;
            this.label12.Text = "Pos X";
            // 
            // KDTreeMinX
            // 
            this.KDTreeMinX.DecimalPlaces = 3;
            this.KDTreeMinX.Location = new System.Drawing.Point(71, 34);
            this.KDTreeMinX.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.KDTreeMinX.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.KDTreeMinX.Name = "KDTreeMinX";
            this.KDTreeMinX.Size = new System.Drawing.Size(120, 20);
            this.KDTreeMinX.TabIndex = 26;
            this.KDTreeMinX.ValueChanged += new System.EventHandler(this.KDTreeMinX_ValueChanged);
            // 
            // KDTreeMax
            // 
            this.KDTreeMax.Controls.Add(this.label7);
            this.KDTreeMax.Controls.Add(this.label8);
            this.KDTreeMax.Controls.Add(this.KDTreeMaxZ);
            this.KDTreeMax.Controls.Add(this.KDTreeMaxY);
            this.KDTreeMax.Controls.Add(this.label9);
            this.KDTreeMax.Controls.Add(this.KDTreeMaxX);
            this.KDTreeMax.Location = new System.Drawing.Point(6, 516);
            this.KDTreeMax.Name = "KDTreeMax";
            this.KDTreeMax.Size = new System.Drawing.Size(197, 132);
            this.KDTreeMax.TabIndex = 32;
            this.KDTreeMax.TabStop = false;
            this.KDTreeMax.Text = "Max";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(19, 100);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(35, 13);
            this.label7.TabIndex = 31;
            this.label7.Text = "Pos Z";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(19, 68);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(35, 13);
            this.label8.TabIndex = 30;
            this.label8.Text = "Pos Y";
            // 
            // KDTreeMaxZ
            // 
            this.KDTreeMaxZ.DecimalPlaces = 3;
            this.KDTreeMaxZ.Location = new System.Drawing.Point(71, 98);
            this.KDTreeMaxZ.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.KDTreeMaxZ.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.KDTreeMaxZ.Name = "KDTreeMaxZ";
            this.KDTreeMaxZ.Size = new System.Drawing.Size(120, 20);
            this.KDTreeMaxZ.TabIndex = 29;
            this.KDTreeMaxZ.ValueChanged += new System.EventHandler(this.KDTreeMaxZ_ValueChanged);
            // 
            // KDTreeMaxY
            // 
            this.KDTreeMaxY.DecimalPlaces = 3;
            this.KDTreeMaxY.Location = new System.Drawing.Point(71, 66);
            this.KDTreeMaxY.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.KDTreeMaxY.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.KDTreeMaxY.Name = "KDTreeMaxY";
            this.KDTreeMaxY.Size = new System.Drawing.Size(120, 20);
            this.KDTreeMaxY.TabIndex = 28;
            this.KDTreeMaxY.ValueChanged += new System.EventHandler(this.KDTreeMaxY_ValueChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(19, 36);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(35, 13);
            this.label9.TabIndex = 27;
            this.label9.Text = "Pos X";
            // 
            // KDTreeMaxX
            // 
            this.KDTreeMaxX.DecimalPlaces = 3;
            this.KDTreeMaxX.Location = new System.Drawing.Point(71, 34);
            this.KDTreeMaxX.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.KDTreeMaxX.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.KDTreeMaxX.Name = "KDTreeMaxX";
            this.KDTreeMaxX.Size = new System.Drawing.Size(120, 20);
            this.KDTreeMaxX.TabIndex = 26;
            this.KDTreeMaxX.ValueChanged += new System.EventHandler(this.KDTreeMaxX_ValueChanged);
            // 
            // CheckPointInfo
            // 
            this.CheckPointInfo.Controls.Add(this.CheckPointPosZLabel);
            this.CheckPointInfo.Controls.Add(this.CheckPointPosYLabel);
            this.CheckPointInfo.Controls.Add(this.CheckPointPosZ);
            this.CheckPointInfo.Controls.Add(this.CheckPointPosY);
            this.CheckPointInfo.Controls.Add(this.CheckPointPosXLabel);
            this.CheckPointInfo.Controls.Add(this.CheckPointPosX);
            this.CheckPointInfo.Location = new System.Drawing.Point(6, 353);
            this.CheckPointInfo.Name = "CheckPointInfo";
            this.CheckPointInfo.Size = new System.Drawing.Size(197, 132);
            this.CheckPointInfo.TabIndex = 30;
            this.CheckPointInfo.TabStop = false;
            this.CheckPointInfo.Text = "Check Point Info";
            this.CheckPointInfo.Visible = false;
            // 
            // CheckPointPosZLabel
            // 
            this.CheckPointPosZLabel.AutoSize = true;
            this.CheckPointPosZLabel.Location = new System.Drawing.Point(19, 100);
            this.CheckPointPosZLabel.Name = "CheckPointPosZLabel";
            this.CheckPointPosZLabel.Size = new System.Drawing.Size(35, 13);
            this.CheckPointPosZLabel.TabIndex = 31;
            this.CheckPointPosZLabel.Text = "Pos Z";
            // 
            // CheckPointPosYLabel
            // 
            this.CheckPointPosYLabel.AutoSize = true;
            this.CheckPointPosYLabel.Location = new System.Drawing.Point(19, 68);
            this.CheckPointPosYLabel.Name = "CheckPointPosYLabel";
            this.CheckPointPosYLabel.Size = new System.Drawing.Size(35, 13);
            this.CheckPointPosYLabel.TabIndex = 30;
            this.CheckPointPosYLabel.Text = "Pos Y";
            // 
            // CheckPointPosZ
            // 
            this.CheckPointPosZ.DecimalPlaces = 3;
            this.CheckPointPosZ.Location = new System.Drawing.Point(71, 98);
            this.CheckPointPosZ.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.CheckPointPosZ.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.CheckPointPosZ.Name = "CheckPointPosZ";
            this.CheckPointPosZ.Size = new System.Drawing.Size(120, 20);
            this.CheckPointPosZ.TabIndex = 29;
            this.CheckPointPosZ.ValueChanged += new System.EventHandler(this.CheckPointPosZ_ValueChanged);
            // 
            // CheckPointPosY
            // 
            this.CheckPointPosY.DecimalPlaces = 3;
            this.CheckPointPosY.Location = new System.Drawing.Point(71, 66);
            this.CheckPointPosY.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.CheckPointPosY.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.CheckPointPosY.Name = "CheckPointPosY";
            this.CheckPointPosY.Size = new System.Drawing.Size(120, 20);
            this.CheckPointPosY.TabIndex = 28;
            this.CheckPointPosY.ValueChanged += new System.EventHandler(this.CheckPointPosY_ValueChanged);
            // 
            // CheckPointPosXLabel
            // 
            this.CheckPointPosXLabel.AutoSize = true;
            this.CheckPointPosXLabel.Location = new System.Drawing.Point(19, 36);
            this.CheckPointPosXLabel.Name = "CheckPointPosXLabel";
            this.CheckPointPosXLabel.Size = new System.Drawing.Size(35, 13);
            this.CheckPointPosXLabel.TabIndex = 27;
            this.CheckPointPosXLabel.Text = "Pos X";
            // 
            // CheckPointPosX
            // 
            this.CheckPointPosX.DecimalPlaces = 3;
            this.CheckPointPosX.Location = new System.Drawing.Point(71, 34);
            this.CheckPointPosX.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.CheckPointPosX.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.CheckPointPosX.Name = "CheckPointPosX";
            this.CheckPointPosX.Size = new System.Drawing.Size(120, 20);
            this.CheckPointPosX.TabIndex = 26;
            this.CheckPointPosX.ValueChanged += new System.EventHandler(this.CheckPointPosX_ValueChanged);
            // 
            // SaveAll
            // 
            this.SaveAll.Location = new System.Drawing.Point(117, 297);
            this.SaveAll.Name = "SaveAll";
            this.SaveAll.Size = new System.Drawing.Size(75, 23);
            this.SaveAll.TabIndex = 6;
            this.SaveAll.Text = "Save All";
            this.SaveAll.UseVisualStyleBackColor = true;
            this.SaveAll.Click += new System.EventHandler(this.SaveAll_Click);
            // 
            // LevelOpen
            // 
            this.LevelOpen.Location = new System.Drawing.Point(39, 268);
            this.LevelOpen.Name = "LevelOpen";
            this.LevelOpen.Size = new System.Drawing.Size(75, 23);
            this.LevelOpen.TabIndex = 5;
            this.LevelOpen.Text = "Open Level";
            this.LevelOpen.UseVisualStyleBackColor = true;
            this.LevelOpen.Click += new System.EventHandler(this.LevelOpen_Click);
            // 
            // LevelDown
            // 
            this.LevelDown.Location = new System.Drawing.Point(117, 239);
            this.LevelDown.Name = "LevelDown";
            this.LevelDown.Size = new System.Drawing.Size(75, 23);
            this.LevelDown.TabIndex = 4;
            this.LevelDown.Text = "Down";
            this.LevelDown.UseVisualStyleBackColor = true;
            this.LevelDown.Click += new System.EventHandler(this.LevelDown_Click);
            // 
            // LevelDelete
            // 
            this.LevelDelete.Location = new System.Drawing.Point(117, 268);
            this.LevelDelete.Name = "LevelDelete";
            this.LevelDelete.Size = new System.Drawing.Size(75, 23);
            this.LevelDelete.TabIndex = 3;
            this.LevelDelete.Text = "Delete Level";
            this.LevelDelete.UseVisualStyleBackColor = true;
            this.LevelDelete.Click += new System.EventHandler(this.LevelDelete_Click);
            // 
            // LevelUp
            // 
            this.LevelUp.Location = new System.Drawing.Point(39, 239);
            this.LevelUp.Name = "LevelUp";
            this.LevelUp.Size = new System.Drawing.Size(75, 23);
            this.LevelUp.TabIndex = 2;
            this.LevelUp.Text = "Up";
            this.LevelUp.UseVisualStyleBackColor = true;
            this.LevelUp.Click += new System.EventHandler(this.LevelUp_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(39, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(133, 24);
            this.label1.TabIndex = 1;
            this.label1.Text = "Loaded Levels";
            // 
            // LevelList
            // 
            this.LevelList.FormattingEnabled = true;
            this.LevelList.Location = new System.Drawing.Point(6, 34);
            this.LevelList.Name = "LevelList";
            this.LevelList.Size = new System.Drawing.Size(228, 199);
            this.LevelList.TabIndex = 0;
            this.LevelList.SelectedIndexChanged += new System.EventHandler(this.LevelList_SelectedIndexChanged);
            this.LevelList.KeyDown += new System.Windows.Forms.KeyEventHandler(this.LevelList_KeyDown);
            this.LevelList.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.LevelList_KeyPress);
            this.LevelList.KeyUp += new System.Windows.Forms.KeyEventHandler(this.LevelList_KeyDown);
            this.LevelList.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.LevelList_MouseDoubleClick);
            // 
            // ObjPositionsTab
            // 
            this.ObjPositionsTab.Controls.Add(this.label6);
            this.ObjPositionsTab.Controls.Add(this.label5);
            this.ObjPositionsTab.Controls.Add(this.label4);
            this.ObjPositionsTab.Controls.Add(this.label3);
            this.ObjPositionsTab.Controls.Add(this.ObjPosZ);
            this.ObjPositionsTab.Controls.Add(this.ObjPosY);
            this.ObjPositionsTab.Controls.Add(this.ObjPosX);
            this.ObjPositionsTab.Controls.Add(this.ObjTypeBox);
            this.ObjPositionsTab.Controls.Add(this.RemoveObject);
            this.ObjPositionsTab.Controls.Add(this.AddObject);
            this.ObjPositionsTab.Controls.Add(this.label2);
            this.ObjPositionsTab.Controls.Add(this.ObjPositionList);
            this.ObjPositionsTab.Location = new System.Drawing.Point(4, 22);
            this.ObjPositionsTab.Name = "ObjPositionsTab";
            this.ObjPositionsTab.Size = new System.Drawing.Size(240, 838);
            this.ObjPositionsTab.TabIndex = 2;
            this.ObjPositionsTab.Text = "Object Positions";
            this.ObjPositionsTab.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(5, 298);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(31, 13);
            this.label6.TabIndex = 11;
            this.label6.Text = "Type";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(5, 324);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(35, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "Pos X";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(5, 350);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Pos Y";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(5, 376);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Pos Z";
            // 
            // ObjPosZ
            // 
            this.ObjPosZ.DecimalPlaces = 3;
            this.ObjPosZ.Location = new System.Drawing.Point(46, 374);
            this.ObjPosZ.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.ObjPosZ.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.ObjPosZ.Name = "ObjPosZ";
            this.ObjPosZ.Size = new System.Drawing.Size(120, 20);
            this.ObjPosZ.TabIndex = 7;
            this.ObjPosZ.ValueChanged += new System.EventHandler(this.ObjPosZ_ValueChanged);
            // 
            // ObjPosY
            // 
            this.ObjPosY.DecimalPlaces = 3;
            this.ObjPosY.Location = new System.Drawing.Point(46, 348);
            this.ObjPosY.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.ObjPosY.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.ObjPosY.Name = "ObjPosY";
            this.ObjPosY.Size = new System.Drawing.Size(120, 20);
            this.ObjPosY.TabIndex = 6;
            this.ObjPosY.ValueChanged += new System.EventHandler(this.ObjPosY_ValueChanged);
            // 
            // ObjPosX
            // 
            this.ObjPosX.DecimalPlaces = 3;
            this.ObjPosX.Location = new System.Drawing.Point(46, 322);
            this.ObjPosX.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.ObjPosX.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.ObjPosX.Name = "ObjPosX";
            this.ObjPosX.Size = new System.Drawing.Size(120, 20);
            this.ObjPosX.TabIndex = 5;
            this.ObjPosX.ValueChanged += new System.EventHandler(this.ObjPosX_ValueChanged);
            // 
            // ObjTypeBox
            // 
            this.ObjTypeBox.FormattingEnabled = true;
            this.ObjTypeBox.Items.AddRange(new object[] {
            "Crate",
            "Cork",
            "Plug",
            "Pudge",
            "SlimeMonster"});
            this.ObjTypeBox.Location = new System.Drawing.Point(46, 295);
            this.ObjTypeBox.Name = "ObjTypeBox";
            this.ObjTypeBox.Size = new System.Drawing.Size(121, 21);
            this.ObjTypeBox.TabIndex = 4;
            this.ObjTypeBox.SelectedIndexChanged += new System.EventHandler(this.ObjTypeBox_SelectedIndexChanged);
            this.ObjTypeBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ObjTypeBox_KeyDown);
            this.ObjTypeBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ObjTypeBox_KeyPress);
            // 
            // RemoveObject
            // 
            this.RemoveObject.Location = new System.Drawing.Point(124, 227);
            this.RemoveObject.Name = "RemoveObject";
            this.RemoveObject.Size = new System.Drawing.Size(75, 23);
            this.RemoveObject.TabIndex = 3;
            this.RemoveObject.Text = "RemoveObject";
            this.RemoveObject.UseVisualStyleBackColor = true;
            this.RemoveObject.Click += new System.EventHandler(this.RemoveObject_Click);
            // 
            // AddObject
            // 
            this.AddObject.Location = new System.Drawing.Point(32, 227);
            this.AddObject.Name = "AddObject";
            this.AddObject.Size = new System.Drawing.Size(75, 23);
            this.AddObject.TabIndex = 2;
            this.AddObject.Text = "Add Object";
            this.AddObject.UseVisualStyleBackColor = true;
            this.AddObject.Click += new System.EventHandler(this.AddObject_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(81, 36);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(74, 24);
            this.label2.TabIndex = 1;
            this.label2.Text = "Objects";
            // 
            // ObjPositionList
            // 
            this.ObjPositionList.FormattingEnabled = true;
            this.ObjPositionList.Location = new System.Drawing.Point(5, 74);
            this.ObjPositionList.Name = "ObjPositionList";
            this.ObjPositionList.Size = new System.Drawing.Size(232, 147);
            this.ObjPositionList.TabIndex = 0;
            this.ObjPositionList.SelectedIndexChanged += new System.EventHandler(this.ObjPositionList_SelectedIndexChanged);
            this.ObjPositionList.KeyDown += new System.Windows.Forms.KeyEventHandler(this.LevelList_KeyDown);
            this.ObjPositionList.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.LevelList_KeyPress);
            this.ObjPositionList.KeyUp += new System.Windows.Forms.KeyEventHandler(this.LevelList_KeyDown);
            this.ObjPositionList.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.ObjPositionList_MouseDoubleClick);
            // 
            // HazardInfoTab
            // 
            this.HazardInfoTab.Controls.Add(this.ObjsChannelsLabel);
            this.HazardInfoTab.Controls.Add(this.ChannelList);
            this.HazardInfoTab.Controls.Add(this.WaterPoolChanLabel);
            this.HazardInfoTab.Controls.Add(this.HazardChannel);
            this.HazardInfoTab.Location = new System.Drawing.Point(4, 22);
            this.HazardInfoTab.Name = "HazardInfoTab";
            this.HazardInfoTab.Padding = new System.Windows.Forms.Padding(3);
            this.HazardInfoTab.Size = new System.Drawing.Size(240, 838);
            this.HazardInfoTab.TabIndex = 0;
            this.HazardInfoTab.Text = "HazardInfo";
            this.HazardInfoTab.UseVisualStyleBackColor = true;
            // 
            // ObjsChannelsLabel
            // 
            this.ObjsChannelsLabel.AutoSize = true;
            this.ObjsChannelsLabel.Location = new System.Drawing.Point(41, 496);
            this.ObjsChannelsLabel.Name = "ObjsChannelsLabel";
            this.ObjsChannelsLabel.Size = new System.Drawing.Size(154, 13);
            this.ObjsChannelsLabel.TabIndex = 28;
            this.ObjsChannelsLabel.Text = "Objects On The Same Channel";
            this.ObjsChannelsLabel.Visible = false;
            // 
            // ChannelList
            // 
            this.ChannelList.FormattingEnabled = true;
            this.ChannelList.Location = new System.Drawing.Point(7, 522);
            this.ChannelList.Name = "ChannelList";
            this.ChannelList.Size = new System.Drawing.Size(228, 199);
            this.ChannelList.TabIndex = 30;
            // 
            // WaterPoolChanLabel
            // 
            this.WaterPoolChanLabel.AutoSize = true;
            this.WaterPoolChanLabel.Location = new System.Drawing.Point(78, 446);
            this.WaterPoolChanLabel.Name = "WaterPoolChanLabel";
            this.WaterPoolChanLabel.Size = new System.Drawing.Size(46, 13);
            this.WaterPoolChanLabel.TabIndex = 29;
            this.WaterPoolChanLabel.Text = "Channel";
            // 
            // HazardChannel
            // 
            this.HazardChannel.Location = new System.Drawing.Point(44, 462);
            this.HazardChannel.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.HazardChannel.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.HazardChannel.Name = "HazardChannel";
            this.HazardChannel.Size = new System.Drawing.Size(120, 20);
            this.HazardChannel.TabIndex = 28;
            this.HazardChannel.Value = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.HazardChannel.ValueChanged += new System.EventHandler(this.HazardChannel_ValueChanged);
            // 
            // LightInfoTab
            // 
            this.LightInfoTab.Controls.Add(this.RemoveLight);
            this.LightInfoTab.Controls.Add(this.AddLight);
            this.LightInfoTab.Controls.Add(this.label16);
            this.LightInfoTab.Controls.Add(this.label15);
            this.LightInfoTab.Controls.Add(this.SpecularIntensity);
            this.LightInfoTab.Controls.Add(this.SpecularPower);
            this.LightInfoTab.Controls.Add(this.SpecularButton);
            this.LightInfoTab.Controls.Add(this.AmbientButton);
            this.LightInfoTab.Controls.Add(this.DiffuseButton);
            this.LightInfoTab.Controls.Add(this.DirLightAtt);
            this.LightInfoTab.Controls.Add(this.label14);
            this.LightInfoTab.Controls.Add(this.LightType);
            this.LightInfoTab.Controls.Add(this.label13);
            this.LightInfoTab.Controls.Add(this.LevelLights);
            this.LightInfoTab.Location = new System.Drawing.Point(4, 22);
            this.LightInfoTab.Name = "LightInfoTab";
            this.LightInfoTab.Size = new System.Drawing.Size(240, 838);
            this.LightInfoTab.TabIndex = 3;
            this.LightInfoTab.Text = "Light Info";
            this.LightInfoTab.UseVisualStyleBackColor = true;
            // 
            // RemoveLight
            // 
            this.RemoveLight.Location = new System.Drawing.Point(123, 276);
            this.RemoveLight.Name = "RemoveLight";
            this.RemoveLight.Size = new System.Drawing.Size(84, 23);
            this.RemoveLight.TabIndex = 21;
            this.RemoveLight.Text = "Remove Light";
            this.RemoveLight.UseVisualStyleBackColor = true;
            this.RemoveLight.Click += new System.EventHandler(this.RemoveLight_Click);
            // 
            // AddLight
            // 
            this.AddLight.Location = new System.Drawing.Point(22, 276);
            this.AddLight.Name = "AddLight";
            this.AddLight.Size = new System.Drawing.Size(75, 23);
            this.AddLight.TabIndex = 20;
            this.AddLight.Text = "Add Light";
            this.AddLight.UseVisualStyleBackColor = true;
            this.AddLight.Click += new System.EventHandler(this.AddLight_Click);
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(10, 460);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(91, 13);
            this.label16.TabIndex = 20;
            this.label16.Text = "Specular Intensity";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(19, 433);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(82, 13);
            this.label15.TabIndex = 19;
            this.label15.Text = "Specular Power";
            // 
            // SpecularIntensity
            // 
            this.SpecularIntensity.DecimalPlaces = 5;
            this.SpecularIntensity.Location = new System.Drawing.Point(107, 458);
            this.SpecularIntensity.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.SpecularIntensity.Name = "SpecularIntensity";
            this.SpecularIntensity.Size = new System.Drawing.Size(120, 20);
            this.SpecularIntensity.TabIndex = 18;
            this.SpecularIntensity.ValueChanged += new System.EventHandler(this.SpecularIntensity_ValueChanged);
            // 
            // SpecularPower
            // 
            this.SpecularPower.DecimalPlaces = 5;
            this.SpecularPower.Location = new System.Drawing.Point(107, 431);
            this.SpecularPower.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.SpecularPower.Name = "SpecularPower";
            this.SpecularPower.Size = new System.Drawing.Size(120, 20);
            this.SpecularPower.TabIndex = 17;
            this.SpecularPower.ValueChanged += new System.EventHandler(this.SpecularPower_ValueChanged);
            // 
            // SpecularButton
            // 
            this.SpecularButton.Location = new System.Drawing.Point(68, 390);
            this.SpecularButton.Name = "SpecularButton";
            this.SpecularButton.Size = new System.Drawing.Size(106, 23);
            this.SpecularButton.TabIndex = 16;
            this.SpecularButton.Text = "Specular Color";
            this.SpecularButton.UseVisualStyleBackColor = true;
            this.SpecularButton.Click += new System.EventHandler(this.SpecularButton_Click);
            // 
            // AmbientButton
            // 
            this.AmbientButton.Location = new System.Drawing.Point(132, 355);
            this.AmbientButton.Name = "AmbientButton";
            this.AmbientButton.Size = new System.Drawing.Size(106, 23);
            this.AmbientButton.TabIndex = 15;
            this.AmbientButton.Text = "Ambient Color";
            this.AmbientButton.UseVisualStyleBackColor = true;
            this.AmbientButton.Click += new System.EventHandler(this.AmbientButton_Click);
            // 
            // DiffuseButton
            // 
            this.DiffuseButton.Location = new System.Drawing.Point(4, 355);
            this.DiffuseButton.Name = "DiffuseButton";
            this.DiffuseButton.Size = new System.Drawing.Size(106, 23);
            this.DiffuseButton.TabIndex = 0;
            this.DiffuseButton.Text = "Diffuse Color";
            this.DiffuseButton.UseVisualStyleBackColor = true;
            this.DiffuseButton.Click += new System.EventHandler(this.DiffuseButton_Click);
            // 
            // DirLightAtt
            // 
            this.DirLightAtt.Controls.Add(this.DirectionalDirZ);
            this.DirLightAtt.Controls.Add(this.DirectionalDirY);
            this.DirLightAtt.Controls.Add(this.DirectionalDirX);
            this.DirLightAtt.Controls.Add(this.label19);
            this.DirLightAtt.Controls.Add(this.label18);
            this.DirLightAtt.Controls.Add(this.label17);
            this.DirLightAtt.Location = new System.Drawing.Point(6, 498);
            this.DirLightAtt.Name = "DirLightAtt";
            this.DirLightAtt.Size = new System.Drawing.Size(232, 120);
            this.DirLightAtt.TabIndex = 14;
            this.DirLightAtt.TabStop = false;
            this.DirLightAtt.Text = "Directional Light Attr.";
            this.DirLightAtt.Visible = false;
            // 
            // DirectionalDirZ
            // 
            this.DirectionalDirZ.DecimalPlaces = 6;
            this.DirectionalDirZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.DirectionalDirZ.Location = new System.Drawing.Point(101, 81);
            this.DirectionalDirZ.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.DirectionalDirZ.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.DirectionalDirZ.Name = "DirectionalDirZ";
            this.DirectionalDirZ.Size = new System.Drawing.Size(120, 20);
            this.DirectionalDirZ.TabIndex = 17;
            this.DirectionalDirZ.ValueChanged += new System.EventHandler(this.DirectionalDirZ_ValueChanged);
            // 
            // DirectionalDirY
            // 
            this.DirectionalDirY.DecimalPlaces = 6;
            this.DirectionalDirY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.DirectionalDirY.Location = new System.Drawing.Point(101, 55);
            this.DirectionalDirY.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.DirectionalDirY.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.DirectionalDirY.Name = "DirectionalDirY";
            this.DirectionalDirY.Size = new System.Drawing.Size(120, 20);
            this.DirectionalDirY.TabIndex = 17;
            this.DirectionalDirY.ValueChanged += new System.EventHandler(this.DirectionalDirY_ValueChanged);
            // 
            // DirectionalDirX
            // 
            this.DirectionalDirX.DecimalPlaces = 6;
            this.DirectionalDirX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.DirectionalDirX.Location = new System.Drawing.Point(101, 30);
            this.DirectionalDirX.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.DirectionalDirX.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.DirectionalDirX.Name = "DirectionalDirX";
            this.DirectionalDirX.Size = new System.Drawing.Size(120, 20);
            this.DirectionalDirX.TabIndex = 17;
            this.DirectionalDirX.ValueChanged += new System.EventHandler(this.DirectionalDirX_ValueChanged);
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(13, 83);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(59, 13);
            this.label19.TabIndex = 19;
            this.label19.Text = "Direction Z";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(13, 57);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(59, 13);
            this.label18.TabIndex = 19;
            this.label18.Text = "Direction Y";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(13, 32);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(59, 13);
            this.label17.TabIndex = 19;
            this.label17.Text = "Direction X";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(24, 318);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(31, 13);
            this.label14.TabIndex = 13;
            this.label14.Text = "Type";
            // 
            // LightType
            // 
            this.LightType.FormattingEnabled = true;
            this.LightType.Items.AddRange(new object[] {
            "Directional",
            "Point",
            "Spot"});
            this.LightType.Location = new System.Drawing.Point(65, 315);
            this.LightType.Name = "LightType";
            this.LightType.Size = new System.Drawing.Size(121, 21);
            this.LightType.TabIndex = 12;
            this.LightType.SelectedIndexChanged += new System.EventHandler(this.LightType_SelectedIndexChanged);
            this.LightType.KeyDown += new System.Windows.Forms.KeyEventHandler(this.LightType_KeyDown);
            this.LightType.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.LightType_KeyPress);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(70, 35);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(59, 24);
            this.label13.TabIndex = 2;
            this.label13.Text = "Lights";
            // 
            // LevelLights
            // 
            this.LevelLights.FormattingEnabled = true;
            this.LevelLights.Location = new System.Drawing.Point(5, 66);
            this.LevelLights.Name = "LevelLights";
            this.LevelLights.Size = new System.Drawing.Size(232, 199);
            this.LevelLights.TabIndex = 0;
            this.LevelLights.SelectedIndexChanged += new System.EventHandler(this.LevelLights_SelectedIndexChanged);
            this.LevelLights.KeyDown += new System.Windows.Forms.KeyEventHandler(this.LevelList_KeyDown);
            this.LevelLights.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.LevelList_KeyPress);
            this.LevelLights.KeyUp += new System.Windows.Forms.KeyEventHandler(this.LevelList_KeyDown);
            this.LevelLights.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.LevelLights_MouseDoubleClick);
            // 
            // DisplayPanel
            // 
            this.DisplayPanel.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.DisplayPanel.Controls.Add(this.SpotLightAttr);
            this.DisplayPanel.Controls.Add(this.PointLightAttr);
            this.DisplayPanel.Controls.Add(this.BoxSpawnInfo);
            this.DisplayPanel.Controls.Add(this.DoorInfo);
            this.DisplayPanel.Controls.Add(this.DoorButtonInfo);
            this.DisplayPanel.Controls.Add(this.BoxButtonInfo);
            this.DisplayPanel.Controls.Add(this.GooInfo);
            this.DisplayPanel.Controls.Add(this.CrusherInfo);
            this.DisplayPanel.Controls.Add(this.BridgeInfo);
            this.DisplayPanel.Controls.Add(this.OutletInfo);
            this.DisplayPanel.Controls.Add(this.SpawnInfo);
            this.DisplayPanel.Controls.Add(this.SteamInfo);
            this.DisplayPanel.Controls.Add(this.WaterPoolInfo);
            this.DisplayPanel.Controls.Add(this.IntakeInfo);
            this.DisplayPanel.Controls.Add(this.BreakableWallInfo);
            this.DisplayPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DisplayPanel.Location = new System.Drawing.Point(0, 0);
            this.DisplayPanel.Name = "DisplayPanel";
            this.DisplayPanel.Size = new System.Drawing.Size(957, 892);
            this.DisplayPanel.TabIndex = 0;
            // 
            // SpotLightAttr
            // 
            this.SpotLightAttr.Controls.Add(this.label32);
            this.SpotLightAttr.Controls.Add(this.label31);
            this.SpotLightAttr.Controls.Add(this.SpotRadius);
            this.SpotLightAttr.Controls.Add(this.label33);
            this.SpotLightAttr.Controls.Add(this.SpotAttenZ);
            this.SpotLightAttr.Controls.Add(this.SpotAttenY);
            this.SpotLightAttr.Controls.Add(this.SpotAttenX);
            this.SpotLightAttr.Controls.Add(this.label30);
            this.SpotLightAttr.Controls.Add(this.label27);
            this.SpotLightAttr.Controls.Add(this.SpotExponent);
            this.SpotLightAttr.Controls.Add(this.SpotExponentLabel);
            this.SpotLightAttr.Controls.Add(this.label28);
            this.SpotLightAttr.Controls.Add(this.SpotCutOff);
            this.SpotLightAttr.Controls.Add(this.SpotCutOffLabel);
            this.SpotLightAttr.Controls.Add(this.label29);
            this.SpotLightAttr.Controls.Add(this.SpotDirZ);
            this.SpotLightAttr.Controls.Add(this.SpotPosZ);
            this.SpotLightAttr.Controls.Add(this.SpotDirY);
            this.SpotLightAttr.Controls.Add(this.SpotDirX);
            this.SpotLightAttr.Controls.Add(this.SpotPosY);
            this.SpotLightAttr.Controls.Add(this.SpotDirZLabel);
            this.SpotLightAttr.Controls.Add(this.SpotDirYLabel);
            this.SpotLightAttr.Controls.Add(this.SpotPosX);
            this.SpotLightAttr.Controls.Add(this.SpotDirXLabel);
            this.SpotLightAttr.Location = new System.Drawing.Point(247, 560);
            this.SpotLightAttr.Name = "SpotLightAttr";
            this.SpotLightAttr.Size = new System.Drawing.Size(227, 332);
            this.SpotLightAttr.TabIndex = 2;
            this.SpotLightAttr.TabStop = false;
            this.SpotLightAttr.Text = "Spot Light Attr";
            this.SpotLightAttr.Visible = false;
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(6, 175);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(71, 13);
            this.label32.TabIndex = 56;
            this.label32.Text = "Attenuation X";
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Location = new System.Drawing.Point(8, 200);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(71, 13);
            this.label31.TabIndex = 55;
            this.label31.Text = "Attenuation Y";
            // 
            // SpotRadius
            // 
            this.SpotRadius.DecimalPlaces = 6;
            this.SpotRadius.Location = new System.Drawing.Point(85, 257);
            this.SpotRadius.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.SpotRadius.Name = "SpotRadius";
            this.SpotRadius.Size = new System.Drawing.Size(120, 20);
            this.SpotRadius.TabIndex = 53;
            this.SpotRadius.ValueChanged += new System.EventHandler(this.SpotRadius_ValueChanged);
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(30, 259);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(40, 13);
            this.label33.TabIndex = 54;
            this.label33.Text = "Radius";
            // 
            // SpotAttenZ
            // 
            this.SpotAttenZ.DecimalPlaces = 6;
            this.SpotAttenZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.SpotAttenZ.Location = new System.Drawing.Point(85, 224);
            this.SpotAttenZ.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SpotAttenZ.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.SpotAttenZ.Name = "SpotAttenZ";
            this.SpotAttenZ.Size = new System.Drawing.Size(120, 20);
            this.SpotAttenZ.TabIndex = 49;
            this.SpotAttenZ.ValueChanged += new System.EventHandler(this.SpotAttenZ_ValueChanged);
            // 
            // SpotAttenY
            // 
            this.SpotAttenY.DecimalPlaces = 6;
            this.SpotAttenY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.SpotAttenY.Location = new System.Drawing.Point(85, 198);
            this.SpotAttenY.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SpotAttenY.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.SpotAttenY.Name = "SpotAttenY";
            this.SpotAttenY.Size = new System.Drawing.Size(120, 20);
            this.SpotAttenY.TabIndex = 48;
            this.SpotAttenY.ValueChanged += new System.EventHandler(this.SpotAttenY_ValueChanged);
            // 
            // SpotAttenX
            // 
            this.SpotAttenX.DecimalPlaces = 6;
            this.SpotAttenX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.SpotAttenX.Location = new System.Drawing.Point(85, 173);
            this.SpotAttenX.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SpotAttenX.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.SpotAttenX.Name = "SpotAttenX";
            this.SpotAttenX.Size = new System.Drawing.Size(120, 20);
            this.SpotAttenX.TabIndex = 47;
            this.SpotAttenX.ValueChanged += new System.EventHandler(this.SpotAttenX_ValueChanged);
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(8, 226);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(71, 13);
            this.label30.TabIndex = 52;
            this.label30.Text = "Attenuation Z";
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(35, 72);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(35, 13);
            this.label27.TabIndex = 46;
            this.label27.Text = "Pos Z";
            // 
            // SpotExponent
            // 
            this.SpotExponent.DecimalPlaces = 6;
            this.SpotExponent.Location = new System.Drawing.Point(85, 309);
            this.SpotExponent.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.SpotExponent.Name = "SpotExponent";
            this.SpotExponent.Size = new System.Drawing.Size(120, 20);
            this.SpotExponent.TabIndex = 28;
            this.SpotExponent.ValueChanged += new System.EventHandler(this.SpotExponent_ValueChanged);
            // 
            // SpotExponentLabel
            // 
            this.SpotExponentLabel.AutoSize = true;
            this.SpotExponentLabel.Location = new System.Drawing.Point(27, 311);
            this.SpotExponentLabel.Name = "SpotExponentLabel";
            this.SpotExponentLabel.Size = new System.Drawing.Size(52, 13);
            this.SpotExponentLabel.TabIndex = 29;
            this.SpotExponentLabel.Text = "Exponent";
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(35, 47);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(35, 13);
            this.label28.TabIndex = 45;
            this.label28.Text = "Pos Y";
            // 
            // SpotCutOff
            // 
            this.SpotCutOff.DecimalPlaces = 6;
            this.SpotCutOff.Location = new System.Drawing.Point(85, 283);
            this.SpotCutOff.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.SpotCutOff.Name = "SpotCutOff";
            this.SpotCutOff.Size = new System.Drawing.Size(120, 20);
            this.SpotCutOff.TabIndex = 26;
            this.SpotCutOff.ValueChanged += new System.EventHandler(this.SpotCutOff_ValueChanged);
            // 
            // SpotCutOffLabel
            // 
            this.SpotCutOffLabel.AutoSize = true;
            this.SpotCutOffLabel.Location = new System.Drawing.Point(30, 285);
            this.SpotCutOffLabel.Name = "SpotCutOffLabel";
            this.SpotCutOffLabel.Size = new System.Drawing.Size(40, 13);
            this.SpotCutOffLabel.TabIndex = 27;
            this.SpotCutOffLabel.Text = "Cut Off";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(35, 21);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(35, 13);
            this.label29.TabIndex = 44;
            this.label29.Text = "Pos X";
            // 
            // SpotDirZ
            // 
            this.SpotDirZ.DecimalPlaces = 6;
            this.SpotDirZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.SpotDirZ.Location = new System.Drawing.Point(85, 147);
            this.SpotDirZ.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SpotDirZ.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.SpotDirZ.Name = "SpotDirZ";
            this.SpotDirZ.Size = new System.Drawing.Size(120, 20);
            this.SpotDirZ.TabIndex = 22;
            this.SpotDirZ.ValueChanged += new System.EventHandler(this.SpotDirZ_ValueChanged);
            // 
            // SpotPosZ
            // 
            this.SpotPosZ.DecimalPlaces = 5;
            this.SpotPosZ.Location = new System.Drawing.Point(85, 70);
            this.SpotPosZ.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.SpotPosZ.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.SpotPosZ.Name = "SpotPosZ";
            this.SpotPosZ.Size = new System.Drawing.Size(120, 20);
            this.SpotPosZ.TabIndex = 43;
            this.SpotPosZ.ValueChanged += new System.EventHandler(this.SpotPosZ_ValueChanged);
            // 
            // SpotDirY
            // 
            this.SpotDirY.DecimalPlaces = 6;
            this.SpotDirY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.SpotDirY.Location = new System.Drawing.Point(85, 121);
            this.SpotDirY.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SpotDirY.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.SpotDirY.Name = "SpotDirY";
            this.SpotDirY.Size = new System.Drawing.Size(120, 20);
            this.SpotDirY.TabIndex = 21;
            this.SpotDirY.ValueChanged += new System.EventHandler(this.SpotDirY_ValueChanged);
            // 
            // SpotDirX
            // 
            this.SpotDirX.DecimalPlaces = 6;
            this.SpotDirX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.SpotDirX.Location = new System.Drawing.Point(85, 96);
            this.SpotDirX.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SpotDirX.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.SpotDirX.Name = "SpotDirX";
            this.SpotDirX.Size = new System.Drawing.Size(120, 20);
            this.SpotDirX.TabIndex = 20;
            this.SpotDirX.ValueChanged += new System.EventHandler(this.SpotDirX_ValueChanged);
            // 
            // SpotPosY
            // 
            this.SpotPosY.DecimalPlaces = 5;
            this.SpotPosY.Location = new System.Drawing.Point(85, 45);
            this.SpotPosY.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.SpotPosY.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.SpotPosY.Name = "SpotPosY";
            this.SpotPosY.Size = new System.Drawing.Size(120, 20);
            this.SpotPosY.TabIndex = 42;
            this.SpotPosY.ValueChanged += new System.EventHandler(this.SpotPosY_ValueChanged);
            // 
            // SpotDirZLabel
            // 
            this.SpotDirZLabel.AutoSize = true;
            this.SpotDirZLabel.Location = new System.Drawing.Point(18, 149);
            this.SpotDirZLabel.Name = "SpotDirZLabel";
            this.SpotDirZLabel.Size = new System.Drawing.Size(59, 13);
            this.SpotDirZLabel.TabIndex = 25;
            this.SpotDirZLabel.Text = "Direction Z";
            // 
            // SpotDirYLabel
            // 
            this.SpotDirYLabel.AutoSize = true;
            this.SpotDirYLabel.Location = new System.Drawing.Point(18, 123);
            this.SpotDirYLabel.Name = "SpotDirYLabel";
            this.SpotDirYLabel.Size = new System.Drawing.Size(59, 13);
            this.SpotDirYLabel.TabIndex = 24;
            this.SpotDirYLabel.Text = "Direction Y";
            // 
            // SpotPosX
            // 
            this.SpotPosX.DecimalPlaces = 5;
            this.SpotPosX.Location = new System.Drawing.Point(85, 19);
            this.SpotPosX.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.SpotPosX.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.SpotPosX.Name = "SpotPosX";
            this.SpotPosX.Size = new System.Drawing.Size(120, 20);
            this.SpotPosX.TabIndex = 41;
            this.SpotPosX.ValueChanged += new System.EventHandler(this.SpotPosX_ValueChanged);
            // 
            // SpotDirXLabel
            // 
            this.SpotDirXLabel.AutoSize = true;
            this.SpotDirXLabel.Location = new System.Drawing.Point(18, 98);
            this.SpotDirXLabel.Name = "SpotDirXLabel";
            this.SpotDirXLabel.Size = new System.Drawing.Size(59, 13);
            this.SpotDirXLabel.TabIndex = 23;
            this.SpotDirXLabel.Text = "Direction X";
            // 
            // PointLightAttr
            // 
            this.PointLightAttr.Controls.Add(this.label24);
            this.PointLightAttr.Controls.Add(this.label25);
            this.PointLightAttr.Controls.Add(this.label26);
            this.PointLightAttr.Controls.Add(this.PointLAttenZ);
            this.PointLightAttr.Controls.Add(this.PointLAttenY);
            this.PointLightAttr.Controls.Add(this.PointLAttenX);
            this.PointLightAttr.Controls.Add(this.label23);
            this.PointLightAttr.Controls.Add(this.PointLRadius);
            this.PointLightAttr.Controls.Add(this.label22);
            this.PointLightAttr.Controls.Add(this.label21);
            this.PointLightAttr.Controls.Add(this.label20);
            this.PointLightAttr.Controls.Add(this.PointLPosZ);
            this.PointLightAttr.Controls.Add(this.PointLPosY);
            this.PointLightAttr.Controls.Add(this.PointLPosX);
            this.PointLightAttr.Location = new System.Drawing.Point(670, 475);
            this.PointLightAttr.Name = "PointLightAttr";
            this.PointLightAttr.Size = new System.Drawing.Size(200, 242);
            this.PointLightAttr.TabIndex = 1;
            this.PointLightAttr.TabStop = false;
            this.PointLightAttr.Text = "Point Light Attr";
            this.PointLightAttr.Visible = false;
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(6, 201);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(71, 13);
            this.label24.TabIndex = 40;
            this.label24.Text = "Attenuation Z";
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(6, 176);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(71, 13);
            this.label25.TabIndex = 39;
            this.label25.Text = "Attenuation Y";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(6, 150);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(71, 13);
            this.label26.TabIndex = 38;
            this.label26.Text = "Attenuation X";
            // 
            // PointLAttenZ
            // 
            this.PointLAttenZ.DecimalPlaces = 6;
            this.PointLAttenZ.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.PointLAttenZ.Location = new System.Drawing.Point(83, 199);
            this.PointLAttenZ.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.PointLAttenZ.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.PointLAttenZ.Name = "PointLAttenZ";
            this.PointLAttenZ.Size = new System.Drawing.Size(101, 20);
            this.PointLAttenZ.TabIndex = 37;
            this.PointLAttenZ.ValueChanged += new System.EventHandler(this.PointLAttenZ_ValueChanged);
            // 
            // PointLAttenY
            // 
            this.PointLAttenY.DecimalPlaces = 6;
            this.PointLAttenY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.PointLAttenY.Location = new System.Drawing.Point(83, 174);
            this.PointLAttenY.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.PointLAttenY.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.PointLAttenY.Name = "PointLAttenY";
            this.PointLAttenY.Size = new System.Drawing.Size(101, 20);
            this.PointLAttenY.TabIndex = 36;
            this.PointLAttenY.ValueChanged += new System.EventHandler(this.PointLAttenY_ValueChanged);
            // 
            // PointLAttenX
            // 
            this.PointLAttenX.DecimalPlaces = 6;
            this.PointLAttenX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.PointLAttenX.Location = new System.Drawing.Point(83, 148);
            this.PointLAttenX.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.PointLAttenX.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.PointLAttenX.Name = "PointLAttenX";
            this.PointLAttenX.Size = new System.Drawing.Size(101, 20);
            this.PointLAttenX.TabIndex = 35;
            this.PointLAttenX.ValueChanged += new System.EventHandler(this.PointLAttenX_ValueChanged);
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(12, 110);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(40, 13);
            this.label23.TabIndex = 34;
            this.label23.Text = "Radius";
            // 
            // PointLRadius
            // 
            this.PointLRadius.DecimalPlaces = 5;
            this.PointLRadius.Location = new System.Drawing.Point(64, 108);
            this.PointLRadius.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.PointLRadius.Name = "PointLRadius";
            this.PointLRadius.Size = new System.Drawing.Size(120, 20);
            this.PointLRadius.TabIndex = 33;
            this.PointLRadius.ValueChanged += new System.EventHandler(this.PointLRadius_ValueChanged);
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(12, 76);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(35, 13);
            this.label22.TabIndex = 32;
            this.label22.Text = "Pos Z";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(12, 51);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(35, 13);
            this.label21.TabIndex = 31;
            this.label21.Text = "Pos Y";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(12, 25);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(35, 13);
            this.label20.TabIndex = 30;
            this.label20.Text = "Pos X";
            // 
            // PointLPosZ
            // 
            this.PointLPosZ.DecimalPlaces = 5;
            this.PointLPosZ.Location = new System.Drawing.Point(64, 74);
            this.PointLPosZ.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.PointLPosZ.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.PointLPosZ.Name = "PointLPosZ";
            this.PointLPosZ.Size = new System.Drawing.Size(120, 20);
            this.PointLPosZ.TabIndex = 2;
            this.PointLPosZ.ValueChanged += new System.EventHandler(this.PointLPosZ_ValueChanged);
            // 
            // PointLPosY
            // 
            this.PointLPosY.DecimalPlaces = 5;
            this.PointLPosY.Location = new System.Drawing.Point(64, 49);
            this.PointLPosY.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.PointLPosY.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.PointLPosY.Name = "PointLPosY";
            this.PointLPosY.Size = new System.Drawing.Size(120, 20);
            this.PointLPosY.TabIndex = 1;
            this.PointLPosY.ValueChanged += new System.EventHandler(this.PointLPosY_ValueChanged);
            // 
            // PointLPosX
            // 
            this.PointLPosX.DecimalPlaces = 5;
            this.PointLPosX.Location = new System.Drawing.Point(64, 23);
            this.PointLPosX.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.PointLPosX.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.PointLPosX.Name = "PointLPosX";
            this.PointLPosX.Size = new System.Drawing.Size(120, 20);
            this.PointLPosX.TabIndex = 0;
            this.PointLPosX.ValueChanged += new System.EventHandler(this.PointLPosX_ValueChanged);
            // 
            // BoxSpawnInfo
            // 
            this.BoxSpawnInfo.Location = new System.Drawing.Point(670, 358);
            this.BoxSpawnInfo.Name = "BoxSpawnInfo";
            this.BoxSpawnInfo.Size = new System.Drawing.Size(170, 87);
            this.BoxSpawnInfo.TabIndex = 32;
            this.BoxSpawnInfo.TabStop = false;
            this.BoxSpawnInfo.Text = "BoxSpawn Info";
            this.BoxSpawnInfo.Visible = false;
            // 
            // DoorInfo
            // 
            this.DoorInfo.Location = new System.Drawing.Point(646, 249);
            this.DoorInfo.Name = "DoorInfo";
            this.DoorInfo.Size = new System.Drawing.Size(170, 87);
            this.DoorInfo.TabIndex = 31;
            this.DoorInfo.TabStop = false;
            this.DoorInfo.Text = "Door Info";
            this.DoorInfo.Visible = false;
            // 
            // DoorButtonInfo
            // 
            this.DoorButtonInfo.Controls.Add(this.label35);
            this.DoorButtonInfo.Controls.Add(this.DoorButtonWireName);
            this.DoorButtonInfo.Location = new System.Drawing.Point(637, 147);
            this.DoorButtonInfo.Name = "DoorButtonInfo";
            this.DoorButtonInfo.Size = new System.Drawing.Size(170, 87);
            this.DoorButtonInfo.TabIndex = 30;
            this.DoorButtonInfo.TabStop = false;
            this.DoorButtonInfo.Text = "Door Button Info";
            this.DoorButtonInfo.Visible = false;
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.Location = new System.Drawing.Point(25, 25);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(60, 13);
            this.label35.TabIndex = 33;
            this.label35.Text = "Wire Name";
            // 
            // DoorButtonWireName
            // 
            this.DoorButtonWireName.Location = new System.Drawing.Point(28, 42);
            this.DoorButtonWireName.Name = "DoorButtonWireName";
            this.DoorButtonWireName.Size = new System.Drawing.Size(100, 20);
            this.DoorButtonWireName.TabIndex = 32;
            this.DoorButtonWireName.TextChanged += new System.EventHandler(this.DoorButtonWireName_TextChanged);
            // 
            // BoxButtonInfo
            // 
            this.BoxButtonInfo.Controls.Add(this.label34);
            this.BoxButtonInfo.Controls.Add(this.BoxButtonWireName);
            this.BoxButtonInfo.Location = new System.Drawing.Point(637, 36);
            this.BoxButtonInfo.Name = "BoxButtonInfo";
            this.BoxButtonInfo.Size = new System.Drawing.Size(170, 87);
            this.BoxButtonInfo.TabIndex = 29;
            this.BoxButtonInfo.TabStop = false;
            this.BoxButtonInfo.Text = "Box Button Info";
            this.BoxButtonInfo.Visible = false;
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.Location = new System.Drawing.Point(25, 25);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(60, 13);
            this.label34.TabIndex = 33;
            this.label34.Text = "Wire Name";
            // 
            // BoxButtonWireName
            // 
            this.BoxButtonWireName.Location = new System.Drawing.Point(28, 42);
            this.BoxButtonWireName.Name = "BoxButtonWireName";
            this.BoxButtonWireName.Size = new System.Drawing.Size(100, 20);
            this.BoxButtonWireName.TabIndex = 32;
            this.BoxButtonWireName.TextChanged += new System.EventHandler(this.BoxButtonWireName_TextChanged);
            // 
            // GooInfo
            // 
            this.GooInfo.Controls.Add(this.ConnectAllPits);
            this.GooInfo.Controls.Add(this.GooDamageLabel);
            this.GooInfo.Controls.Add(this.GooDamage);
            this.GooInfo.Location = new System.Drawing.Point(57, 500);
            this.GooInfo.Name = "GooInfo";
            this.GooInfo.Size = new System.Drawing.Size(170, 113);
            this.GooInfo.TabIndex = 27;
            this.GooInfo.TabStop = false;
            this.GooInfo.Text = "Goo Pit Info";
            this.GooInfo.Visible = false;
            // 
            // ConnectAllPits
            // 
            this.ConnectAllPits.AutoSize = true;
            this.ConnectAllPits.Location = new System.Drawing.Point(9, 31);
            this.ConnectAllPits.Name = "ConnectAllPits";
            this.ConnectAllPits.Size = new System.Drawing.Size(123, 17);
            this.ConnectAllPits.TabIndex = 30;
            this.ConnectAllPits.Text = "Connect All Goo Pits";
            this.ConnectAllPits.UseVisualStyleBackColor = true;
            this.ConnectAllPits.CheckedChanged += new System.EventHandler(this.ConnectAllPits_CheckedChanged);
            // 
            // GooDamageLabel
            // 
            this.GooDamageLabel.AutoSize = true;
            this.GooDamageLabel.Location = new System.Drawing.Point(6, 62);
            this.GooDamageLabel.Name = "GooDamageLabel";
            this.GooDamageLabel.Size = new System.Drawing.Size(106, 13);
            this.GooDamageLabel.TabIndex = 27;
            this.GooDamageLabel.Text = "Damage Per Second";
            // 
            // GooDamage
            // 
            this.GooDamage.DecimalPlaces = 4;
            this.GooDamage.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.GooDamage.Location = new System.Drawing.Point(9, 78);
            this.GooDamage.Name = "GooDamage";
            this.GooDamage.Size = new System.Drawing.Size(120, 20);
            this.GooDamage.TabIndex = 26;
            this.GooDamage.ValueChanged += new System.EventHandler(this.GooDamage_ValueChanged);
            // 
            // CrusherInfo
            // 
            this.CrusherInfo.Controls.Add(this.CrusherDirLabel);
            this.CrusherInfo.Controls.Add(this.CrusherDir);
            this.CrusherInfo.Controls.Add(this.CrusherDurationLable);
            this.CrusherInfo.Controls.Add(this.CrusherDuration);
            this.CrusherInfo.Controls.Add(this.CrusherFreq);
            this.CrusherInfo.Controls.Add(this.CrusherFreqLable);
            this.CrusherInfo.Controls.Add(this.CrusherLengthLable);
            this.CrusherInfo.Controls.Add(this.CrusherLength);
            this.CrusherInfo.Location = new System.Drawing.Point(480, 362);
            this.CrusherInfo.Name = "CrusherInfo";
            this.CrusherInfo.Size = new System.Drawing.Size(170, 228);
            this.CrusherInfo.TabIndex = 26;
            this.CrusherInfo.TabStop = false;
            this.CrusherInfo.Text = "Crusher Info";
            this.CrusherInfo.Visible = false;
            // 
            // CrusherDirLabel
            // 
            this.CrusherDirLabel.AutoSize = true;
            this.CrusherDirLabel.Location = new System.Drawing.Point(12, 171);
            this.CrusherDirLabel.Name = "CrusherDirLabel";
            this.CrusherDirLabel.Size = new System.Drawing.Size(88, 13);
            this.CrusherDirLabel.TabIndex = 25;
            this.CrusherDirLabel.Text = "Crusher Direction";
            // 
            // CrusherDir
            // 
            this.CrusherDir.DisplayMember = "0";
            this.CrusherDir.FormattingEnabled = true;
            this.CrusherDir.Items.AddRange(new object[] {
            "Up",
            "Down",
            "Left",
            "Right"});
            this.CrusherDir.Location = new System.Drawing.Point(15, 187);
            this.CrusherDir.Name = "CrusherDir";
            this.CrusherDir.Size = new System.Drawing.Size(121, 21);
            this.CrusherDir.TabIndex = 24;
            this.CrusherDir.SelectedIndexChanged += new System.EventHandler(this.CrusherDir_SelectedIndexChanged);
            // 
            // CrusherDurationLable
            // 
            this.CrusherDurationLable.AutoSize = true;
            this.CrusherDurationLable.Location = new System.Drawing.Point(12, 72);
            this.CrusherDurationLable.Name = "CrusherDurationLable";
            this.CrusherDurationLable.Size = new System.Drawing.Size(141, 13);
            this.CrusherDurationLable.TabIndex = 19;
            this.CrusherDurationLable.Text = "Crusher Duration ( seconds )";
            // 
            // CrusherDuration
            // 
            this.CrusherDuration.DecimalPlaces = 4;
            this.CrusherDuration.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.CrusherDuration.Location = new System.Drawing.Point(15, 88);
            this.CrusherDuration.Name = "CrusherDuration";
            this.CrusherDuration.Size = new System.Drawing.Size(120, 20);
            this.CrusherDuration.TabIndex = 18;
            this.CrusherDuration.ValueChanged += new System.EventHandler(this.CrusherDuration_ValueChanged);
            // 
            // CrusherFreq
            // 
            this.CrusherFreq.DecimalPlaces = 4;
            this.CrusherFreq.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.CrusherFreq.Location = new System.Drawing.Point(15, 136);
            this.CrusherFreq.Name = "CrusherFreq";
            this.CrusherFreq.Size = new System.Drawing.Size(120, 20);
            this.CrusherFreq.TabIndex = 20;
            this.CrusherFreq.ValueChanged += new System.EventHandler(this.CrusherFreq_ValueChanged);
            // 
            // CrusherFreqLable
            // 
            this.CrusherFreqLable.AutoSize = true;
            this.CrusherFreqLable.Location = new System.Drawing.Point(12, 120);
            this.CrusherFreqLable.Name = "CrusherFreqLable";
            this.CrusherFreqLable.Size = new System.Drawing.Size(151, 13);
            this.CrusherFreqLable.TabIndex = 21;
            this.CrusherFreqLable.Text = "Crusher Frequency ( seconds )";
            // 
            // CrusherLengthLable
            // 
            this.CrusherLengthLable.AutoSize = true;
            this.CrusherLengthLable.Location = new System.Drawing.Point(12, 28);
            this.CrusherLengthLable.Name = "CrusherLengthLable";
            this.CrusherLengthLable.Size = new System.Drawing.Size(79, 13);
            this.CrusherLengthLable.TabIndex = 17;
            this.CrusherLengthLable.Text = "Crusher Length";
            // 
            // CrusherLength
            // 
            this.CrusherLength.Location = new System.Drawing.Point(15, 44);
            this.CrusherLength.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.CrusherLength.Name = "CrusherLength";
            this.CrusherLength.Size = new System.Drawing.Size(120, 20);
            this.CrusherLength.TabIndex = 16;
            this.CrusherLength.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.CrusherLength.ValueChanged += new System.EventHandler(this.CrusherLength_ValueChanged);
            // 
            // BridgeInfo
            // 
            this.BridgeInfo.Controls.Add(this.BridgeRotate);
            this.BridgeInfo.Controls.Add(this.BridgeQuadLabel);
            this.BridgeInfo.Controls.Add(this.BridgeQuadrant);
            this.BridgeInfo.Location = new System.Drawing.Point(454, 178);
            this.BridgeInfo.Name = "BridgeInfo";
            this.BridgeInfo.Size = new System.Drawing.Size(170, 114);
            this.BridgeInfo.TabIndex = 28;
            this.BridgeInfo.TabStop = false;
            this.BridgeInfo.Text = "Bridge Info";
            this.BridgeInfo.Visible = false;
            // 
            // BridgeRotate
            // 
            this.BridgeRotate.AutoSize = true;
            this.BridgeRotate.Location = new System.Drawing.Point(9, 24);
            this.BridgeRotate.Name = "BridgeRotate";
            this.BridgeRotate.Size = new System.Drawing.Size(118, 17);
            this.BridgeRotate.TabIndex = 26;
            this.BridgeRotate.Text = "Rotate ColockWise";
            this.BridgeRotate.UseVisualStyleBackColor = true;
            this.BridgeRotate.CheckedChanged += new System.EventHandler(this.BridgeRoate_CheckedChanged);
            // 
            // BridgeQuadLabel
            // 
            this.BridgeQuadLabel.AutoSize = true;
            this.BridgeQuadLabel.Location = new System.Drawing.Point(6, 55);
            this.BridgeQuadLabel.Name = "BridgeQuadLabel";
            this.BridgeQuadLabel.Size = new System.Drawing.Size(84, 13);
            this.BridgeQuadLabel.TabIndex = 25;
            this.BridgeQuadLabel.Text = "Bridge Quadrant";
            // 
            // BridgeQuadrant
            // 
            this.BridgeQuadrant.FormattingEnabled = true;
            this.BridgeQuadrant.Items.AddRange(new object[] {
            "0",
            "1",
            "2",
            "3"});
            this.BridgeQuadrant.Location = new System.Drawing.Point(9, 71);
            this.BridgeQuadrant.Name = "BridgeQuadrant";
            this.BridgeQuadrant.Size = new System.Drawing.Size(121, 21);
            this.BridgeQuadrant.TabIndex = 24;
            this.BridgeQuadrant.SelectedIndexChanged += new System.EventHandler(this.BridgeQuadrant_SelectedIndexChanged);
            // 
            // OutletInfo
            // 
            this.OutletInfo.Controls.Add(this.OutletDirLabel);
            this.OutletInfo.Controls.Add(this.OutletDir);
            this.OutletInfo.Location = new System.Drawing.Point(445, 35);
            this.OutletInfo.Name = "OutletInfo";
            this.OutletInfo.Size = new System.Drawing.Size(170, 88);
            this.OutletInfo.TabIndex = 27;
            this.OutletInfo.TabStop = false;
            this.OutletInfo.Text = "Outlet Info";
            this.OutletInfo.Visible = false;
            // 
            // OutletDirLabel
            // 
            this.OutletDirLabel.AutoSize = true;
            this.OutletDirLabel.Location = new System.Drawing.Point(6, 27);
            this.OutletDirLabel.Name = "OutletDirLabel";
            this.OutletDirLabel.Size = new System.Drawing.Size(80, 13);
            this.OutletDirLabel.TabIndex = 25;
            this.OutletDirLabel.Text = "Outlet Direction";
            // 
            // OutletDir
            // 
            this.OutletDir.FormattingEnabled = true;
            this.OutletDir.Items.AddRange(new object[] {
            "Up",
            "Down",
            "Left",
            "Right"});
            this.OutletDir.Location = new System.Drawing.Point(9, 43);
            this.OutletDir.Name = "OutletDir";
            this.OutletDir.Size = new System.Drawing.Size(121, 21);
            this.OutletDir.TabIndex = 24;
            this.OutletDir.SelectedIndexChanged += new System.EventHandler(this.OutletDir_SelectedIndexChanged);
            // 
            // SpawnInfo
            // 
            this.SpawnInfo.Controls.Add(this.SpawnDirLable);
            this.SpawnInfo.Controls.Add(this.SpawnDir);
            this.SpawnInfo.Controls.Add(this.SpawnCorkable);
            this.SpawnInfo.Controls.Add(this.SpawnLimitLable);
            this.SpawnInfo.Controls.Add(this.SpawnLimit);
            this.SpawnInfo.Controls.Add(this.SpawnFreqLable);
            this.SpawnInfo.Controls.Add(this.SpawnFreq);
            this.SpawnInfo.Location = new System.Drawing.Point(267, 333);
            this.SpawnInfo.Name = "SpawnInfo";
            this.SpawnInfo.Size = new System.Drawing.Size(170, 215);
            this.SpawnInfo.TabIndex = 25;
            this.SpawnInfo.TabStop = false;
            this.SpawnInfo.Text = "Spawn Pipe Info";
            this.SpawnInfo.Visible = false;
            // 
            // SpawnDirLable
            // 
            this.SpawnDirLable.AutoSize = true;
            this.SpawnDirLable.Location = new System.Drawing.Point(6, 158);
            this.SpawnDirLable.Name = "SpawnDirLable";
            this.SpawnDirLable.Size = new System.Drawing.Size(85, 13);
            this.SpawnDirLable.TabIndex = 25;
            this.SpawnDirLable.Text = "Spawn Direction";
            // 
            // SpawnDir
            // 
            this.SpawnDir.FormattingEnabled = true;
            this.SpawnDir.Items.AddRange(new object[] {
            "Up",
            "Down",
            "Left",
            "Right"});
            this.SpawnDir.Location = new System.Drawing.Point(9, 174);
            this.SpawnDir.Name = "SpawnDir";
            this.SpawnDir.Size = new System.Drawing.Size(121, 21);
            this.SpawnDir.TabIndex = 24;
            this.SpawnDir.SelectedIndexChanged += new System.EventHandler(this.SpawnDir_SelectedIndexChanged);
            // 
            // SpawnCorkable
            // 
            this.SpawnCorkable.AutoSize = true;
            this.SpawnCorkable.Location = new System.Drawing.Point(9, 29);
            this.SpawnCorkable.Name = "SpawnCorkable";
            this.SpawnCorkable.Size = new System.Drawing.Size(68, 17);
            this.SpawnCorkable.TabIndex = 10;
            this.SpawnCorkable.Text = "Corkable";
            this.SpawnCorkable.UseVisualStyleBackColor = true;
            this.SpawnCorkable.CheckedChanged += new System.EventHandler(this.SpawnCorkable_CheckedChanged);
            // 
            // SpawnLimitLable
            // 
            this.SpawnLimitLable.AutoSize = true;
            this.SpawnLimitLable.Location = new System.Drawing.Point(6, 59);
            this.SpawnLimitLable.Name = "SpawnLimitLable";
            this.SpawnLimitLable.Size = new System.Drawing.Size(64, 13);
            this.SpawnLimitLable.TabIndex = 12;
            this.SpawnLimitLable.Text = "Spawn Limit";
            // 
            // SpawnLimit
            // 
            this.SpawnLimit.Location = new System.Drawing.Point(9, 75);
            this.SpawnLimit.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SpawnLimit.Name = "SpawnLimit";
            this.SpawnLimit.Size = new System.Drawing.Size(120, 20);
            this.SpawnLimit.TabIndex = 11;
            this.SpawnLimit.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SpawnLimit.ValueChanged += new System.EventHandler(this.SpawnLimit_ValueChanged);
            // 
            // SpawnFreqLable
            // 
            this.SpawnFreqLable.AutoSize = true;
            this.SpawnFreqLable.Location = new System.Drawing.Point(6, 108);
            this.SpawnFreqLable.Name = "SpawnFreqLable";
            this.SpawnFreqLable.Size = new System.Drawing.Size(148, 13);
            this.SpawnFreqLable.TabIndex = 14;
            this.SpawnFreqLable.Text = "Spawn Frequency ( seconds )";
            // 
            // SpawnFreq
            // 
            this.SpawnFreq.DecimalPlaces = 4;
            this.SpawnFreq.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.SpawnFreq.Location = new System.Drawing.Point(9, 124);
            this.SpawnFreq.Name = "SpawnFreq";
            this.SpawnFreq.Size = new System.Drawing.Size(120, 20);
            this.SpawnFreq.TabIndex = 13;
            this.SpawnFreq.ValueChanged += new System.EventHandler(this.SpawnFreq_ValueChanged);
            // 
            // SteamInfo
            // 
            this.SteamInfo.Controls.Add(this.SteamDirLabel);
            this.SteamInfo.Controls.Add(this.SteamDir);
            this.SteamInfo.Controls.Add(this.SteamFreqLable);
            this.SteamInfo.Controls.Add(this.SteamFreq);
            this.SteamInfo.Controls.Add(this.SteamDurationLable);
            this.SteamInfo.Controls.Add(this.SteamDuration);
            this.SteamInfo.Controls.Add(this.SteamLengthLable);
            this.SteamInfo.Controls.Add(this.SteamLength);
            this.SteamInfo.Controls.Add(this.SteamConstant);
            this.SteamInfo.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SteamInfo.Location = new System.Drawing.Point(258, 20);
            this.SteamInfo.Name = "SteamInfo";
            this.SteamInfo.Size = new System.Drawing.Size(163, 250);
            this.SteamInfo.TabIndex = 24;
            this.SteamInfo.TabStop = false;
            this.SteamInfo.Text = "Steam Pipe Info";
            this.SteamInfo.Visible = false;
            // 
            // SteamDirLabel
            // 
            this.SteamDirLabel.AutoSize = true;
            this.SteamDirLabel.Location = new System.Drawing.Point(6, 201);
            this.SteamDirLabel.Name = "SteamDirLabel";
            this.SteamDirLabel.Size = new System.Drawing.Size(82, 13);
            this.SteamDirLabel.TabIndex = 23;
            this.SteamDirLabel.Text = "Steam Direction";
            // 
            // SteamDir
            // 
            this.SteamDir.DisplayMember = "1";
            this.SteamDir.FormattingEnabled = true;
            this.SteamDir.Items.AddRange(new object[] {
            "Up",
            "Down",
            "Left",
            "Right"});
            this.SteamDir.Location = new System.Drawing.Point(9, 217);
            this.SteamDir.Name = "SteamDir";
            this.SteamDir.Size = new System.Drawing.Size(121, 21);
            this.SteamDir.TabIndex = 22;
            this.SteamDir.SelectedIndexChanged += new System.EventHandler(this.SteamDir_SelectedIndexChanged);
            // 
            // SteamFreqLable
            // 
            this.SteamFreqLable.AutoSize = true;
            this.SteamFreqLable.Location = new System.Drawing.Point(6, 152);
            this.SteamFreqLable.Name = "SteamFreqLable";
            this.SteamFreqLable.Size = new System.Drawing.Size(145, 13);
            this.SteamFreqLable.TabIndex = 8;
            this.SteamFreqLable.Text = "Steam Frequency ( seconds )";
            // 
            // SteamFreq
            // 
            this.SteamFreq.DecimalPlaces = 4;
            this.SteamFreq.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.SteamFreq.Location = new System.Drawing.Point(9, 168);
            this.SteamFreq.Name = "SteamFreq";
            this.SteamFreq.Size = new System.Drawing.Size(120, 20);
            this.SteamFreq.TabIndex = 7;
            this.SteamFreq.ValueChanged += new System.EventHandler(this.SteamFreq_ValueChanged);
            // 
            // SteamDurationLable
            // 
            this.SteamDurationLable.AutoSize = true;
            this.SteamDurationLable.Location = new System.Drawing.Point(6, 106);
            this.SteamDurationLable.Name = "SteamDurationLable";
            this.SteamDurationLable.Size = new System.Drawing.Size(135, 13);
            this.SteamDurationLable.TabIndex = 5;
            this.SteamDurationLable.Text = "Steam Duration ( seconds )";
            // 
            // SteamDuration
            // 
            this.SteamDuration.DecimalPlaces = 4;
            this.SteamDuration.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.SteamDuration.Location = new System.Drawing.Point(9, 122);
            this.SteamDuration.Name = "SteamDuration";
            this.SteamDuration.Size = new System.Drawing.Size(120, 20);
            this.SteamDuration.TabIndex = 4;
            this.SteamDuration.ValueChanged += new System.EventHandler(this.SteamDuration_ValueChanged);
            // 
            // SteamLengthLable
            // 
            this.SteamLengthLable.AutoSize = true;
            this.SteamLengthLable.Location = new System.Drawing.Point(6, 56);
            this.SteamLengthLable.Name = "SteamLengthLable";
            this.SteamLengthLable.Size = new System.Drawing.Size(146, 13);
            this.SteamLengthLable.TabIndex = 3;
            this.SteamLengthLable.Text = "Steam Length ( Pudge Units )";
            // 
            // SteamLength
            // 
            this.SteamLength.Location = new System.Drawing.Point(9, 72);
            this.SteamLength.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SteamLength.Name = "SteamLength";
            this.SteamLength.Size = new System.Drawing.Size(120, 20);
            this.SteamLength.TabIndex = 2;
            this.SteamLength.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SteamLength.ValueChanged += new System.EventHandler(this.SteamLength_ValueChanged);
            // 
            // SteamConstant
            // 
            this.SteamConstant.AutoSize = true;
            this.SteamConstant.Location = new System.Drawing.Point(9, 23);
            this.SteamConstant.Name = "SteamConstant";
            this.SteamConstant.Size = new System.Drawing.Size(68, 17);
            this.SteamConstant.TabIndex = 6;
            this.SteamConstant.Text = "Constant";
            this.SteamConstant.UseVisualStyleBackColor = true;
            this.SteamConstant.CheckedChanged += new System.EventHandler(this.SteamConstant_CheckedChanged);
            // 
            // WaterPoolInfo
            // 
            this.WaterPoolInfo.Controls.Add(this.PoolMeshLabel);
            this.WaterPoolInfo.Controls.Add(this.PoolMeshName);
            this.WaterPoolInfo.Controls.Add(this.WaterPoolHealLabel);
            this.WaterPoolInfo.Controls.Add(this.WaterPoolHeal);
            this.WaterPoolInfo.Location = new System.Drawing.Point(57, 358);
            this.WaterPoolInfo.Name = "WaterPoolInfo";
            this.WaterPoolInfo.Size = new System.Drawing.Size(170, 123);
            this.WaterPoolInfo.TabIndex = 29;
            this.WaterPoolInfo.TabStop = false;
            this.WaterPoolInfo.Text = "WaterPool Info";
            this.WaterPoolInfo.Visible = false;
            // 
            // PoolMeshLabel
            // 
            this.PoolMeshLabel.AutoSize = true;
            this.PoolMeshLabel.Location = new System.Drawing.Point(9, 69);
            this.PoolMeshLabel.Name = "PoolMeshLabel";
            this.PoolMeshLabel.Size = new System.Drawing.Size(120, 13);
            this.PoolMeshLabel.TabIndex = 31;
            this.PoolMeshLabel.Text = "Water Pool Mesh Name";
            // 
            // PoolMeshName
            // 
            this.PoolMeshName.Location = new System.Drawing.Point(12, 86);
            this.PoolMeshName.Name = "PoolMeshName";
            this.PoolMeshName.Size = new System.Drawing.Size(100, 20);
            this.PoolMeshName.TabIndex = 30;
            this.PoolMeshName.TextChanged += new System.EventHandler(this.PoolMeshName_TextChanged);
            // 
            // WaterPoolHealLabel
            // 
            this.WaterPoolHealLabel.AutoSize = true;
            this.WaterPoolHealLabel.Location = new System.Drawing.Point(6, 25);
            this.WaterPoolHealLabel.Name = "WaterPoolHealLabel";
            this.WaterPoolHealLabel.Size = new System.Drawing.Size(121, 13);
            this.WaterPoolHealLabel.TabIndex = 25;
            this.WaterPoolHealLabel.Text = "WaterPool Heal Amount";
            // 
            // WaterPoolHeal
            // 
            this.WaterPoolHeal.DecimalPlaces = 3;
            this.WaterPoolHeal.Location = new System.Drawing.Point(9, 41);
            this.WaterPoolHeal.Name = "WaterPoolHeal";
            this.WaterPoolHeal.Size = new System.Drawing.Size(120, 20);
            this.WaterPoolHeal.TabIndex = 24;
            this.WaterPoolHeal.ValueChanged += new System.EventHandler(this.WaterPoolHeal_ValueChanged);
            // 
            // IntakeInfo
            // 
            this.IntakeInfo.Controls.Add(this.FlowPipeLabel);
            this.IntakeInfo.Controls.Add(this.FlowPipeName);
            this.IntakeInfo.Controls.Add(this.IntakeDirLabel);
            this.IntakeInfo.Controls.Add(this.IntakeDir);
            this.IntakeInfo.Controls.Add(this.IntakeCorkable);
            this.IntakeInfo.Location = new System.Drawing.Point(57, 126);
            this.IntakeInfo.Name = "IntakeInfo";
            this.IntakeInfo.Size = new System.Drawing.Size(170, 166);
            this.IntakeInfo.TabIndex = 26;
            this.IntakeInfo.TabStop = false;
            this.IntakeInfo.Text = "Steam Pipe Intake Info";
            this.IntakeInfo.Visible = false;
            // 
            // FlowPipeLabel
            // 
            this.FlowPipeLabel.AutoSize = true;
            this.FlowPipeLabel.Location = new System.Drawing.Point(6, 111);
            this.FlowPipeLabel.Name = "FlowPipeLabel";
            this.FlowPipeLabel.Size = new System.Drawing.Size(84, 13);
            this.FlowPipeLabel.TabIndex = 27;
            this.FlowPipeLabel.Text = "Flow Pipe Name";
            // 
            // FlowPipeName
            // 
            this.FlowPipeName.Location = new System.Drawing.Point(9, 128);
            this.FlowPipeName.Name = "FlowPipeName";
            this.FlowPipeName.Size = new System.Drawing.Size(100, 20);
            this.FlowPipeName.TabIndex = 26;
            this.FlowPipeName.TextChanged += new System.EventHandler(this.FlowPipeName_TextChanged);
            // 
            // IntakeDirLabel
            // 
            this.IntakeDirLabel.AutoSize = true;
            this.IntakeDirLabel.Location = new System.Drawing.Point(6, 58);
            this.IntakeDirLabel.Name = "IntakeDirLabel";
            this.IntakeDirLabel.Size = new System.Drawing.Size(139, 13);
            this.IntakeDirLabel.TabIndex = 25;
            this.IntakeDirLabel.Text = "Steam Pipe Intake Direction";
            // 
            // IntakeDir
            // 
            this.IntakeDir.FormattingEnabled = true;
            this.IntakeDir.Items.AddRange(new object[] {
            "Up",
            "Down",
            "Left",
            "Right"});
            this.IntakeDir.Location = new System.Drawing.Point(9, 74);
            this.IntakeDir.Name = "IntakeDir";
            this.IntakeDir.Size = new System.Drawing.Size(121, 21);
            this.IntakeDir.TabIndex = 24;
            this.IntakeDir.SelectedIndexChanged += new System.EventHandler(this.IntakeDir_SelectedIndexChanged);
            // 
            // IntakeCorkable
            // 
            this.IntakeCorkable.AutoSize = true;
            this.IntakeCorkable.Location = new System.Drawing.Point(9, 29);
            this.IntakeCorkable.Name = "IntakeCorkable";
            this.IntakeCorkable.Size = new System.Drawing.Size(68, 17);
            this.IntakeCorkable.TabIndex = 10;
            this.IntakeCorkable.Text = "Corkable";
            this.IntakeCorkable.UseVisualStyleBackColor = true;
            this.IntakeCorkable.CheckedChanged += new System.EventHandler(this.IntakeCorkable_CheckedChanged);
            // 
            // BreakableWallInfo
            // 
            this.BreakableWallInfo.Location = new System.Drawing.Point(57, 25);
            this.BreakableWallInfo.Name = "BreakableWallInfo";
            this.BreakableWallInfo.Size = new System.Drawing.Size(170, 87);
            this.BreakableWallInfo.TabIndex = 28;
            this.BreakableWallInfo.TabStop = false;
            this.BreakableWallInfo.Text = "Breakable Wall Info";
            this.BreakableWallInfo.Visible = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1214, 892);
            this.Controls.Add(this.splitContainer1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.MainTabPanel.ResumeLayout(false);
            this.LevelsTab.ResumeLayout(false);
            this.LevelsTab.PerformLayout();
            this.Min.ResumeLayout(false);
            this.Min.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMinZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMinY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMinX)).EndInit();
            this.KDTreeMax.ResumeLayout(false);
            this.KDTreeMax.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMaxZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMaxY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.KDTreeMaxX)).EndInit();
            this.CheckPointInfo.ResumeLayout(false);
            this.CheckPointInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.CheckPointPosZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.CheckPointPosY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.CheckPointPosX)).EndInit();
            this.ObjPositionsTab.ResumeLayout(false);
            this.ObjPositionsTab.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ObjPosZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ObjPosY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ObjPosX)).EndInit();
            this.HazardInfoTab.ResumeLayout(false);
            this.HazardInfoTab.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.HazardChannel)).EndInit();
            this.LightInfoTab.ResumeLayout(false);
            this.LightInfoTab.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SpecularIntensity)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpecularPower)).EndInit();
            this.DirLightAtt.ResumeLayout(false);
            this.DirLightAtt.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DirectionalDirZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.DirectionalDirY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.DirectionalDirX)).EndInit();
            this.DisplayPanel.ResumeLayout(false);
            this.SpotLightAttr.ResumeLayout(false);
            this.SpotLightAttr.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SpotRadius)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotAttenZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotAttenY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotAttenX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotExponent)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotCutOff)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotDirZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotPosZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotDirY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotDirX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotPosY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpotPosX)).EndInit();
            this.PointLightAttr.ResumeLayout(false);
            this.PointLightAttr.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PointLAttenZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLAttenY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLAttenX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLRadius)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLPosZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLPosY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PointLPosX)).EndInit();
            this.DoorButtonInfo.ResumeLayout(false);
            this.DoorButtonInfo.PerformLayout();
            this.BoxButtonInfo.ResumeLayout(false);
            this.BoxButtonInfo.PerformLayout();
            this.GooInfo.ResumeLayout(false);
            this.GooInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.GooDamage)).EndInit();
            this.CrusherInfo.ResumeLayout(false);
            this.CrusherInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.CrusherDuration)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.CrusherFreq)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.CrusherLength)).EndInit();
            this.BridgeInfo.ResumeLayout(false);
            this.BridgeInfo.PerformLayout();
            this.OutletInfo.ResumeLayout(false);
            this.OutletInfo.PerformLayout();
            this.SpawnInfo.ResumeLayout(false);
            this.SpawnInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SpawnLimit)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpawnFreq)).EndInit();
            this.SteamInfo.ResumeLayout(false);
            this.SteamInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SteamFreq)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SteamDuration)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SteamLength)).EndInit();
            this.WaterPoolInfo.ResumeLayout(false);
            this.WaterPoolInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.WaterPoolHeal)).EndInit();
            this.IntakeInfo.ResumeLayout(false);
            this.IntakeInfo.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        public System.Windows.Forms.Panel DisplayPanel;
        private System.Windows.Forms.Label SteamFreqLable;
        private System.Windows.Forms.CheckBox SteamConstant;
        private System.Windows.Forms.Label SteamDurationLable;
        private System.Windows.Forms.NumericUpDown SteamDuration;
        private System.Windows.Forms.Label SteamLengthLable;
        private System.Windows.Forms.NumericUpDown SteamLength;
        private System.Windows.Forms.NumericUpDown SteamFreq;
        private System.Windows.Forms.CheckBox SpawnCorkable;
        private System.Windows.Forms.Label SpawnLimitLable;
        private System.Windows.Forms.NumericUpDown SpawnLimit;
        private System.Windows.Forms.Label SpawnFreqLable;
        private System.Windows.Forms.NumericUpDown SpawnFreq;
        private System.Windows.Forms.Label CrusherDurationLable;
        private System.Windows.Forms.NumericUpDown CrusherDuration;
        private System.Windows.Forms.Label CrusherLengthLable;
        private System.Windows.Forms.NumericUpDown CrusherLength;
        private System.Windows.Forms.Label CrusherFreqLable;
        private System.Windows.Forms.NumericUpDown CrusherFreq;
        private System.Windows.Forms.Label SteamDirLabel;
        private System.Windows.Forms.ComboBox SteamDir;
        private System.Windows.Forms.GroupBox SteamInfo;
        private System.Windows.Forms.GroupBox SpawnInfo;
        private System.Windows.Forms.Label SpawnDirLable;
        private System.Windows.Forms.ComboBox SpawnDir;
        private System.Windows.Forms.GroupBox CrusherInfo;
        private System.Windows.Forms.Label CrusherDirLabel;
        private System.Windows.Forms.ComboBox CrusherDir;
        private System.Windows.Forms.GroupBox IntakeInfo;
        private System.Windows.Forms.Label IntakeDirLabel;
        private System.Windows.Forms.ComboBox IntakeDir;
        private System.Windows.Forms.CheckBox IntakeCorkable;
        private System.Windows.Forms.GroupBox GooInfo;
        private System.Windows.Forms.Label GooDamageLabel;
        private System.Windows.Forms.NumericUpDown GooDamage;
        private System.Windows.Forms.GroupBox OutletInfo;
        private System.Windows.Forms.Label OutletDirLabel;
        private System.Windows.Forms.ComboBox OutletDir;
        private System.Windows.Forms.GroupBox BridgeInfo;
        private System.Windows.Forms.CheckBox BridgeRotate;
        private System.Windows.Forms.Label BridgeQuadLabel;
        private System.Windows.Forms.ComboBox BridgeQuadrant;
        private System.Windows.Forms.GroupBox WaterPoolInfo;
        private System.Windows.Forms.Label WaterPoolHealLabel;
        private System.Windows.Forms.NumericUpDown WaterPoolHeal;
        private System.Windows.Forms.TabControl MainTabPanel;
        private System.Windows.Forms.TabPage HazardInfoTab;
        private System.Windows.Forms.TabPage LevelsTab;
        private System.Windows.Forms.TabPage ObjPositionsTab;
        private System.Windows.Forms.ListBox LevelList;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ListBox ObjPositionList;
        private System.Windows.Forms.Button LevelDown;
        private System.Windows.Forms.Button LevelDelete;
        private System.Windows.Forms.Button LevelUp;
        private System.Windows.Forms.Button LevelOpen;
        private System.Windows.Forms.Button SaveAll;
        private System.Windows.Forms.ComboBox ObjTypeBox;
        private System.Windows.Forms.Button RemoveObject;
        private System.Windows.Forms.Button AddObject;
        private System.Windows.Forms.TabPage LightInfoTab;
        private System.Windows.Forms.NumericUpDown ObjPosZ;
        private System.Windows.Forms.NumericUpDown ObjPosY;
        private System.Windows.Forms.NumericUpDown ObjPosX;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ListBox ChannelList;
        private System.Windows.Forms.GroupBox BreakableWallInfo;
        private System.Windows.Forms.Label ObjsChannelsLabel;
        private System.Windows.Forms.Label WaterPoolChanLabel;
        private System.Windows.Forms.NumericUpDown HazardChannel;
        private System.Windows.Forms.GroupBox BoxButtonInfo;
        private System.Windows.Forms.GroupBox DoorButtonInfo;
        private System.Windows.Forms.GroupBox BoxSpawnInfo;
        private System.Windows.Forms.GroupBox DoorInfo;
        private System.Windows.Forms.CheckBox ConnectAllPits;
        private System.Windows.Forms.TextBox FlowPipeName;
        private System.Windows.Forms.Label FlowPipeLabel;
        private System.Windows.Forms.GroupBox CheckPointInfo;
        private System.Windows.Forms.Label CheckPointPosZLabel;
        private System.Windows.Forms.Label CheckPointPosYLabel;
        private System.Windows.Forms.NumericUpDown CheckPointPosZ;
        private System.Windows.Forms.NumericUpDown CheckPointPosY;
        private System.Windows.Forms.Label CheckPointPosXLabel;
        private System.Windows.Forms.NumericUpDown CheckPointPosX;
        private System.Windows.Forms.GroupBox Min;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.NumericUpDown KDTreeMinZ;
        private System.Windows.Forms.NumericUpDown KDTreeMinY;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.NumericUpDown KDTreeMinX;
        private System.Windows.Forms.GroupBox KDTreeMax;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown KDTreeMaxZ;
        private System.Windows.Forms.NumericUpDown KDTreeMaxY;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown KDTreeMaxX;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.ComboBox LightType;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ListBox LevelLights;
        private System.Windows.Forms.Button RemoveLight;
        private System.Windows.Forms.Button AddLight;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.NumericUpDown SpecularIntensity;
        private System.Windows.Forms.NumericUpDown SpecularPower;
        private System.Windows.Forms.Button SpecularButton;
        private System.Windows.Forms.Button AmbientButton;
        private System.Windows.Forms.Button DiffuseButton;
        private System.Windows.Forms.GroupBox DirLightAtt;
        private System.Windows.Forms.NumericUpDown DirectionalDirZ;
        private System.Windows.Forms.NumericUpDown DirectionalDirY;
        private System.Windows.Forms.NumericUpDown DirectionalDirX;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.GroupBox SpotLightAttr;
        private System.Windows.Forms.GroupBox PointLightAttr;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.NumericUpDown PointLPosZ;
        private System.Windows.Forms.NumericUpDown PointLPosY;
        private System.Windows.Forms.NumericUpDown PointLPosX;
        private System.Windows.Forms.NumericUpDown SpotExponent;
        private System.Windows.Forms.Label SpotExponentLabel;
        private System.Windows.Forms.NumericUpDown SpotCutOff;
        private System.Windows.Forms.Label SpotCutOffLabel;
        private System.Windows.Forms.NumericUpDown SpotDirZ;
        private System.Windows.Forms.NumericUpDown SpotDirY;
        private System.Windows.Forms.NumericUpDown SpotDirX;
        private System.Windows.Forms.Label SpotDirZLabel;
        private System.Windows.Forms.Label SpotDirYLabel;
        private System.Windows.Forms.Label SpotDirXLabel;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.NumericUpDown PointLAttenZ;
        private System.Windows.Forms.NumericUpDown PointLAttenY;
        private System.Windows.Forms.NumericUpDown PointLAttenX;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.NumericUpDown PointLRadius;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.NumericUpDown SpotPosZ;
        private System.Windows.Forms.NumericUpDown SpotPosY;
        private System.Windows.Forms.NumericUpDown SpotPosX;
        private System.Windows.Forms.Label PoolMeshLabel;
        private System.Windows.Forms.TextBox PoolMeshName;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.NumericUpDown SpotRadius;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.NumericUpDown SpotAttenZ;
        private System.Windows.Forms.NumericUpDown SpotAttenY;
        private System.Windows.Forms.NumericUpDown SpotAttenX;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.Button OpenAll;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.TextBox DoorButtonWireName;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.TextBox BoxButtonWireName;
    }
}

