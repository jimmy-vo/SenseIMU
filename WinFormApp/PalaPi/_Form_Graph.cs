using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Timers;
using System.Net;
using ZedGraph;

namespace SENSE
{
    public partial class _Form_Graph : Form
    {

        private int Graph_timezero;
        private double Graph_timetick;
        private enum GRAPH_SCALEMODE { Center, Compact, Scroll, Free }

        GRAPH_SCALEMODE Graph_SCaleMode = GRAPH_SCALEMODE.Center;
        private enum GRAPH_TYPE { TwoDimension, Realtime, Both }

        private delegate void Graph_UpdateForm_Callback(GRAPH_TYPE Type, GRAPH_SCALEMODE mode);

        struct STREAM_DATA
        {
            public double value;
            public string name;
            public string unit;
            public RollingPointPairList PairList;
            public LineItem Curve;
            public Color color;
            public SymbolType Point;
            public Int32 buffer;
            public string Format;
        }

        static STREAM_DATA[] StreamData = new STREAM_DATA[36];

        struct GRAPH_DATA
        {
            public string name;
            public int DataIndex;
            public int size;
            public GRAPH_TYPE Type;
        }

        static GRAPH_DATA EULER_ANGLE =     new GRAPH_DATA { name = "Euler Angle  ", DataIndex = 0, size = 3, Type = GRAPH_TYPE.Realtime };
        static GRAPH_DATA GYROMETER =       new GRAPH_DATA { name = "Gyrometer    ", DataIndex = 21, size = 3, Type = GRAPH_TYPE.Realtime };
        static GRAPH_DATA MAGNETOMETER =    new GRAPH_DATA { name = "Magnetometer ", DataIndex = 24, size = 3, Type = GRAPH_TYPE.Both };
        static GRAPH_DATA ACCELEROMETER = new GRAPH_DATA { name = "Accelerometer", DataIndex = 27, size = 3, Type = GRAPH_TYPE.Realtime };
        static GRAPH_DATA VELOCITY = new GRAPH_DATA { name = "Linear Velocity", DataIndex = 30, size = 3, Type = GRAPH_TYPE.Both };
        static GRAPH_DATA POSITION = new GRAPH_DATA { name = "Linear Position", DataIndex = 33, size = 3, Type = GRAPH_TYPE.Both };

        static GRAPH_DATA[] GRAPH_Data_List = new GRAPH_DATA[6] 
        {
            EULER_ANGLE,
            GYROMETER,
            MAGNETOMETER,
            ACCELEROMETER,
            VELOCITY,
            POSITION
        };

        GRAPH_DATA GRAPH_Data;

        public void GRAPH_Update_StreamData(int index, double value)
        {
            StreamData[index].value = value;
            DATAGrid.Rows[index].Cells["Value"].Value = value;
            if(index>= StreamData.Length-1)            Graph_Sampling();
        }

        public void Graph_Init()
        {
            //Debug("GRAPH: Init");

            //Graph pane
            Graph.GraphPane.IsFontsScaled = false;

            //title
            Graph.GraphPane.Title.IsVisible = false;

            //background fill
            Graph.GraphPane.Chart.Fill = new Fill(Color.Black);
            Graph.GraphPane.Fill = new Fill(Color.Black);

            //legend
            Graph.GraphPane.Legend.Position = LegendPos.InsideTopLeft;
            Graph.GraphPane.Legend.Fill = new Fill(Color.Transparent, Color.Gray);
            Graph.GraphPane.Legend.Fill.AlignV = AlignV.Center;
            Graph.GraphPane.Legend.Fill.AlignH = AlignH.Center;
            Graph.GraphPane.Legend.IsHStack = true;
            Graph.GraphPane.Legend.FontSpec.FontColor = Color.White;
            Graph.GraphPane.Legend.FontSpec.Size = 18;

            //axis 
            Graph.GraphPane.XAxis.IsVisible = true;
            Graph.GraphPane.XAxis.Color = Color.White;
            Graph.GraphPane.XAxis.Title.IsVisible = false;
            Graph.GraphPane.XAxis.Scale.MajorStepAuto = true;
            Graph.GraphPane.XAxis.Scale.FontSpec.FontColor = Color.White;
            Graph.GraphPane.XAxis.Scale.Align = AlignP.Center;
            Graph.GraphPane.XAxis.Scale.AlignH = AlignH.Center;
            Graph.GraphPane.XAxis.Scale.IsLabelsInside = false;
            Graph.GraphPane.XAxis.MajorGrid.IsVisible = false;
            Graph.GraphPane.XAxis.MajorTic.Color = Color.White;
            Graph.GraphPane.XAxis.MinorTic.IsAllTics = false;

            Graph.GraphPane.YAxis.IsVisible = true;
            Graph.GraphPane.YAxis.Color = Color.White;
            Graph.GraphPane.YAxis.Title.IsVisible = false;
            Graph.GraphPane.YAxis.Scale.MajorStepAuto = true;
            Graph.GraphPane.YAxis.Scale.FontSpec.FontColor = Color.White;
            Graph.GraphPane.YAxis.Scale.Align = AlignP.Center;
            Graph.GraphPane.YAxis.Scale.AlignH = AlignH.Center;
            Graph.GraphPane.YAxis.Scale.IsLabelsInside = false;
            Graph.GraphPane.YAxis.MajorGrid.IsVisible = true;
            Graph.GraphPane.YAxis.MajorGrid.Color = Color.WhiteSmoke;
            Graph.GraphPane.YAxis.MajorTic.Color = Color.White;
            Graph.GraphPane.YAxis.MinorTic.IsAllTics = false;


            //axis scale
            Graph.GraphPane.XAxis.ScaleFormatEvent += new Axis.ScaleFormatHandler(Graph_Axis_ScaleEvent);
            Graph.GraphPane.YAxis.ScaleFormatEvent += new Axis.ScaleFormatHandler(Graph_Axis_ScaleEvent);

            Graph.AxisChange();

            Graph.RestoreScale(Graph.GraphPane);
        }

        private void Graph_Form_Process(GRAPH_TYPE Type, GRAPH_SCALEMODE mode)
        {
            switch (Type)
            {
                case GRAPH_TYPE.TwoDimension:
                    List_Graph_ScaleMode.Enabled = false;
                    num_Graph_Interval.Enabled = false;
                    check_Graph_Realtime.Checked = false;
                    check_Graph_XY.Enabled = true;
                    check_Graph_XZ.Enabled = true;
                    check_Graph_YZ.Enabled = true;
                    break;
                case GRAPH_TYPE.Realtime:
                    List_Graph_ScaleMode.Enabled = true;
                    num_Graph_Interval.Enabled = true;
                    check_Graph_XY.Enabled = false;
                    check_Graph_XZ.Enabled = false;
                    check_Graph_YZ.Enabled = false;
                    break;
                case GRAPH_TYPE.Both:
                    List_Graph_ScaleMode.Enabled = true;
                    num_Graph_Interval.Enabled = true;
                    check_Graph_XY.Enabled = true;
                    check_Graph_XZ.Enabled = true;
                    check_Graph_YZ.Enabled = true;
                    break;
            }

            switch (mode)
            {
                case GRAPH_SCALEMODE.Center:
                    group_Interval.Enabled = true;
                    Graph_SCaleMode = mode;
                    break;
                case GRAPH_SCALEMODE.Compact:
                    group_Interval.Enabled = false;
                    Graph_SCaleMode = mode;
                    break;
                case GRAPH_SCALEMODE.Scroll:
                    group_Interval.Enabled = true;
                    Graph_SCaleMode = mode;
                    break;
                case GRAPH_SCALEMODE.Free:
                    group_Interval.Enabled = false;
                    Graph_SCaleMode = mode;
                    break;
            }
        }

        private void Graph_Form_Update(GRAPH_TYPE Type, GRAPH_SCALEMODE mode)
        {
            this.Invoke(new Graph_UpdateForm_Callback(this.Graph_Form_Process), new object[] { Type, mode });
        }

        public _Form_Graph(string[] name, string[] unit, Color[] color, SymbolType[] Point, Int32[] buffer, string[] Format)
        {
            InitializeComponent();
            for (int cnt = 0; cnt < GRAPH_Data_List.Length; cnt++)
                List_Graph_DATA.Items.Add(GRAPH_Data_List[cnt].name);

            for (int cnt = 0; cnt < StreamData.Length; cnt++)
            {
                StreamData[cnt].name = name[cnt];
                StreamData[cnt].unit = unit[cnt];
                StreamData[cnt].color = color[cnt];
                StreamData[cnt].Point = Point[cnt];
                StreamData[cnt].buffer = buffer[cnt];
                StreamData[cnt].Format = Format[cnt];
                DATAGrid.Rows.Add(StreamData[cnt].name, "0", StreamData[cnt].unit);
                DATAGrid.Rows[cnt].Cells["Value"].Style.Format = StreamData[cnt].Format;
            }

            Graph_Init();
            //List_Graph_DATA.SelectedIndex = 0;
        }

        public void Graph_Sampling()
        {
            if (check_Graph_Realtime.Checked) Graph_RealtimePlot();
            else Graph_2DPlot();
        }

        private void Graph_RealtimePlot()
        {
            Graph_timetick = (Environment.TickCount - Graph_timezero) / 1000.0;

            for (int i = 0; i < GRAPH_Data.size; i++)
            {
                if (Graph.GraphPane.CurveList.Count <= 0) return;

                // Get the first CurveItem in the graph
                LineItem curve = Graph.GraphPane.CurveList[i] as LineItem;
                if (curve == null) return;

                // Get the PointPairList
                IPointListEdit list = curve.Points as IPointListEdit;

                // If this is null, it means the reference at curve.Points does not support IPointListEdit, so we won't be able to modify it
                if (list == null) return;

                list.Add(Graph_timetick, StreamData[GRAPH_Data.DataIndex + i].value);
            }

            switch (Graph_SCaleMode)
            {
                case GRAPH_SCALEMODE.Scroll:
                    Graph.GraphPane.XAxis.Scale.Max = Graph_timetick;
                    Graph.GraphPane.XAxis.Scale.Min = Graph.GraphPane.XAxis.Scale.Max - ((double)num_Graph_Interval.Value) / 1000.0;
                    break;
                case GRAPH_SCALEMODE.Compact:
                    Graph.GraphPane.XAxis.Scale.Max = Graph_timetick;
                    Graph.GraphPane.XAxis.Scale.Min = 0;
                    break;
                case GRAPH_SCALEMODE.Center:
                    Graph.GraphPane.XAxis.Scale.Max = Graph_timetick + ((double)num_Graph_Interval.Value) / 2 / 1000;
                    Graph.GraphPane.XAxis.Scale.Min = Graph_timetick - ((double)num_Graph_Interval.Value) / 2 / 1000;
                    break;
                case GRAPH_SCALEMODE.Free:
                    break;
            }
            Graph.Invalidate();
        }

        private void Graph_PlotCircle(double radius, uint step)
        {
            double x, y;
            Graph.GraphPane.CurveList[1].Clear();
            for (x = -radius; x < radius; x += step)
            {
                if (x > radius) x = radius;
                y = Math.Sqrt(radius * radius - x * x);
                if (Graph.GraphPane.CurveList.Count <= 0) return;

                // Get the first CurveItem in the graph
                LineItem curve = Graph.GraphPane.CurveList[1] as LineItem;
                if (curve == null) return;

                // Get the PointPairList
                IPointListEdit list = curve.Points as IPointListEdit;

                // If this is null, it means the reference at curve.Points does not support IPointListEdit, so we won't be able to modify it
                if (list == null) return;

                list.Add(x, y);
                list.Add(x, -y);

                Graph_Compare_Curve.Line.IsVisible = false;
            }
        }

        private void Graph_2DPlot()
        {
            if (Graph.GraphPane.CurveList.Count <= 0)
                return;

            // Get the first CurveItem in the graph
            LineItem curve = Graph.GraphPane.CurveList[0] as LineItem;
            if (curve == null) return;

            // Get the PointPairList
            IPointListEdit list = curve.Points as IPointListEdit;

            // If this is null, it means the reference at curve.Points does not support IPointListEdit, so we won't be able to modify it
            if (list == null) return;

            if (check_Graph_XY.Checked) list.Add(StreamData[GRAPH_Data.DataIndex].value, StreamData[GRAPH_Data.DataIndex + 1].value);
            else if (check_Graph_XZ.Checked) list.Add(StreamData[GRAPH_Data.DataIndex].value, StreamData[GRAPH_Data.DataIndex + 2].value);
            else if (check_Graph_YZ.Checked) list.Add(StreamData[GRAPH_Data.DataIndex + 1].value, StreamData[GRAPH_Data.DataIndex + 2].value);

            Graph_2D_Curve.Line.IsVisible = true;
            Graph.Invalidate();
        }

        public LineItem Graph_2D_Curve;
        public LineItem Graph_Compare_Curve;
        private void Graph_ScaleMode_Update(object sender, EventArgs e)
        {

            switch (List_Graph_ScaleMode.SelectedItem as string)
            {
                case "Compact":
                    Graph_Form_Update(GRAPH_Data.Type, GRAPH_SCALEMODE.Compact);
                    //Debug("GRAPH_Parameter: ->Compact MODE");
                    break;
                case "Scroll":
                    Graph_Form_Update(GRAPH_Data.Type, GRAPH_SCALEMODE.Scroll);
                    //Debug("GRAPH_Parameter: ->Scroll MODE");
                    break;
                case "Center":
                    Graph_Form_Update(GRAPH_Data.Type, GRAPH_SCALEMODE.Center);
                    //Debug("GRAPH_Parameter: ->Center MODE");
                    break;
                case "Free":
                    //Debug("GRAPH_Parameter: ->Free MODE");
                    Graph_Form_Update(GRAPH_Data.Type, GRAPH_SCALEMODE.Free);
                    break;
            }

            Graph.Invalidate();
            Graph.RestoreScale(Graph.GraphPane);
        }

        private void Graph_Filter_Update(object sender, EventArgs e)
        {

            GRAPH_Data = GRAPH_Data_List[List_Graph_DATA.SelectedIndex];
            //Debug("GRAPH_Filter: Switched to " + GRAPH_Data.name);

            //Debug("GRAPH: Clear current plot");
            Graph.GraphPane.CurveList.Clear();
            Graph.GraphPane.GraphObjList.Clear();

            this.Invoke(new EventHandler(Graph_ScaleMode_Update));

            //Debug("GRAPH: Attemp to create curve");

            if (GRAPH_Data.Type == GRAPH_TYPE.Realtime) check_Graph_Realtime.Checked = true;

            if (check_Graph_Realtime.Checked)
            {
                for (int i = 0; i < GRAPH_Data.size; i++)
                {
                    StreamData[GRAPH_Data.DataIndex + i].PairList = new RollingPointPairList(StreamData[GRAPH_Data.DataIndex + i].buffer);
                    StreamData[GRAPH_Data.DataIndex + i].Curve =
                        Graph.GraphPane.AddCurve
                                                (
                                                    StreamData[GRAPH_Data.DataIndex + i].name,
                                                    StreamData[GRAPH_Data.DataIndex + i].PairList,
                                                    StreamData[GRAPH_Data.DataIndex + i].color,
                                                    StreamData[GRAPH_Data.DataIndex + i].Point
                                                );
                    //Debug("GRAPH: " + Protocol_StreamData[GRAPH_Data.DataIndex + i].name + "'s curve created");
                }
            }
            else
            {
                RollingPointPairList Graph_2D_PairList = new RollingPointPairList(60000);

                if (check_Graph_XY.Checked) Graph_2D_Curve = Graph.GraphPane.AddCurve("XY", Graph_2D_PairList, Color.DarkGreen, SymbolType.Diamond);
                else if (check_Graph_XZ.Checked) Graph_2D_Curve = Graph.GraphPane.AddCurve("XZ", Graph_2D_PairList, Color.DarkGreen, SymbolType.Diamond);
                else if (check_Graph_YZ.Checked) Graph_2D_Curve = Graph.GraphPane.AddCurve("YZ", Graph_2D_PairList, Color.DarkGreen, SymbolType.Diamond);

                RollingPointPairList Graph_Compare_PairList = new RollingPointPairList(60000);
                Graph_Compare_Curve = Graph.GraphPane.AddCurve("Circle", Graph_Compare_PairList, Color.Red, SymbolType.Diamond);
                //Debug("GRAPH: 2D's curve created");
            }

            //Debug("GRAPH: Reset TickCount");
            Graph_timezero = Environment.TickCount;
        }
        
        private void Graph_MouseClick(object sender, MouseEventArgs e)
        {
            if (!check_Graph_Realtime.Checked)
            {
                double x, y;
                Graph.GraphPane.ReverseTransform(new PointF(e.X, e.Y), out x, out y);
                Graph_PlotCircle(Math.Sqrt(x * x + y * y), 1);
            }
            else
            {
                Graph_Double_Click.X = e.X;
                Graph_Double_Click.Y = e.Y;
            }
        }

        PointF Graph_Double_Click = new PointF(0, 0);

        private void Graph_DoubleClick(object sender, MouseEventArgs e)
        {
            Graph.RestoreScale(Graph.GraphPane);
        }

        string Graph_Axis_ScaleEvent(GraphPane pane, Axis axis, double val, int index)
        {
            if (val > 1000 || val < -1000)
            {
                return String.Format("{0}k", (int)(val / 1000));
            }
            else
                return String.Format("{0}", val);
        }
    }
}
