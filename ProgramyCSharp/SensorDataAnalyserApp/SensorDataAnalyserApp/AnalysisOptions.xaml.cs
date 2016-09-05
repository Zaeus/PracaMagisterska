using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace SensorDataAnalyserApp
{
    /// <summary>
    /// Interaction logic for AnalysisOptions.xaml
    /// </summary>
    public partial class AnalysisOptions : Window
    {
        /// <summary>
        /// Czy przycisk Rozpocznij... został wciśnięty
        /// </summary>
        private bool _isOKButtonClicked = false;

        /// <summary>
        /// Czy przycisk Anuluj został wciśnięty
        /// </summary>
        private bool _isCancelButtonClicked = false;

        /// <summary>
        /// Wybrana ilość miejsc po przecinku do analizy
        /// </summary>
        public int Digits { get; private set; }

        /// <summary>
        /// Czy dane przechowywane przez histogram mają być wymazane
        /// </summary>
        public bool? ClearHistogramBeforeAnalysis { get; private set; } = true;

        public AnalysisOptions()
        {
            InitializeComponent();
        }

        public bool? ShowAnalysisOptionDialog()
        {
            ShowDialog();
            if (_isOKButtonClicked)
            {
                return true;
            }
            else if (_isCancelButtonClicked)
            {
                return false;
            }
            else
            {
                return null;
            }
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            _isOKButtonClicked = true;

            Close();
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            _isCancelButtonClicked = true;

            Close();
        }

        private void NumberOfDigitsComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Digits = int.Parse(((ComboBoxItem)NumberOfDigitsComboBox.SelectedValue).Content.ToString());
        }

        private void ClearHistogramBeforeCheckBox_Click(object sender, RoutedEventArgs e)
        {
            ClearHistogramBeforeAnalysis = ClearHistogramBeforeCheckBox.IsChecked;

            if (ClearHistogramBeforeCheckBox.IsChecked.HasValue ? ClearHistogramBeforeCheckBox.IsChecked.Value : false)
            {
                ClearHistogramBeforeCheckBox.Content = "Tak";
            }
            else
            {
                ClearHistogramBeforeCheckBox.Content = "Nie";
            }
        }
    }
}
