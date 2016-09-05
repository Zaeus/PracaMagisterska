using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.DataVisualization.Charting;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SensorDataAnalyserApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string[] _listOfFiles;

        private FileCheckerAndLoader _fileCheckerAndLoader;

        private SortedDictionary<string, uint> histogram;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void LoadData_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog _openFileDialog = new OpenFileDialog();
            _openFileDialog.Title = "Wybierz pliki do wczytania. Pliki muszą pochodzić od tego samego czujnika!";
            _openFileDialog.Filter = "Text Files (*.txt)|*.txt";
            _openFileDialog.Multiselect = true;

            bool result = _openFileDialog.ShowDialog() ?? false;

            if (result)
            {
                _listOfFiles = _openFileDialog.FileNames;
                LoadedFilesTextBox.Text = string.Join(Environment.NewLine, _listOfFiles);
                _fileCheckerAndLoader = new FileCheckerAndLoader(_listOfFiles);

                if (_fileCheckerAndLoader.IsAllFileFromTheSameSensor())
                {
                    AnalyseStartButton.IsEnabled = AnalyseStartMenuItem.IsEnabled = true;
                }
                else
                {
                    return;
                }
            }
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {

        }
        
        private void AnalyseStart_Click(object sender, RoutedEventArgs e)
        {
            AnalysisOptions _analysisOptions = new AnalysisOptions();
            bool? _result = _analysisOptions.ShowAnalysisOptionDialog();

            if (_result.HasValue ? _result.Value : false)
            {
                histogram = _fileCheckerAndLoader.LoadDataAndComputeHistogram(_analysisOptions.Digits);
                HorizontalHistogramDataTextBox.Text = VerticalHistogramDataTextBox.Text = HelperComponents.DictionaryToString(histogram);

                BarSeries _barSeries = new BarSeries();
                _barSeries.Title = "Rejestrowane przez czujnik zmiany temperatury";
                _barSeries.DependentValuePath = "Value";
                _barSeries.IndependentValuePath = "Key";
                _barSeries.ItemsSource = histogram;

                HorizontalHistogramBarChart.Series.Add(_barSeries);

                ColumnSeries _columnSeries = new ColumnSeries();
                _columnSeries.Title = "Rejestrowane przez czujnik zmiany temperatury";
                _columnSeries.DependentValuePath = "Value";
                _columnSeries.IndependentValuePath = "Key";
                _columnSeries.ItemsSource = histogram;

                VerticalHistogramBarChart.Series.Add(_columnSeries);
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {

        }

        private void RefreshHistogram_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
