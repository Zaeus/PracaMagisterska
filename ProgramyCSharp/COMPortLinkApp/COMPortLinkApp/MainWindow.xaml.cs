using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.IO.Ports;
using System.Windows;

namespace COMPortLinkApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// Komendy Rozpoczęcia i Zatrzymania dla mikrokontrolera
        /// </summary>
        private readonly string START_CMD = "START_CMD";
        private readonly string STOP_CMD = "STOP_CMD";

        /// <summary>
        /// Tablica nazwa dostępnych na komputerze portów COM
        /// </summary>
        private string[] _portNames;

        /// <summary>
        /// Tablica przechowująca wszystkie otrzymane wiadomości
        /// </summary>
        private List<Tuple<DateTime, string>> _allMessages;

        /// <summary>
        /// Wyświetlany tekst
        /// </summary>
        private string _allMessagesInline;

        /// <summary>
        /// Dostępne prędkości transmisji
        /// </summary>
        private string[] _availablePortBaudRates = { "110", "300", "600", "1200", "2400", "4800", "9600",
            "14400", "19200", "28800", "38400", "56000", "57600", "115200" };

        /// <summary>
        /// Instancja klasy SerialPort, będącej odpowiedzialnej za komunikację COM
        /// </summary>
        private SerialPort _serialPort;

        /// <summary>
        /// Wartość sprawdzająca czy aktualnie pobrane dane zostały zapisane
        /// W przypadku TRUE program nie przypomni się przy zamykaniu o zapisanie danych
        /// W przypadku FALSE program przypomni się przy zamykaniu o zapisanie danych
        /// </summary>
        private bool _isCurrentSessionSaved;

        /// <summary>
        /// Czy zawartość TextBox ma być zawsze przewijana do końca zawartości
        /// </summary>
        private bool _scrollToTheEnd;
        
        /// <summary>
        /// Konstruktor domyślny okna MainWindow
        /// Inicjalizuje talicę nazw portów COM
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();

            _allMessages = new List<Tuple<DateTime, string>>();
            _allMessagesInline = string.Empty;

            BaudRateComboBox.ItemsSource = _availablePortBaudRates;
            BaudRateComboBox.SelectedItem = "9600";

            try
            {
                _portNames = SerialPort.GetPortNames();
                COMPortsComboBox.ItemsSource = _portNames;
                COMPortsComboBox.SelectedIndex = 0;
            }
            catch (Exception ex)
            {
                ReceivingMessageTextBox.Text += "Nie udało się pobrać listy portów COM!" + Environment.NewLine;
                ReceivingMessageTextBox.Text += ex.Message + Environment.NewLine;
                ReceivingMessageTextBox.Text += ex.Source + Environment.NewLine;
                ReceivingMessageTextBox.Text += ex.StackTrace + Environment.NewLine;
            }
        }

        /// <summary>
        /// Obsługa zdarzenia kliknięcia w przycisk Startu
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            if (BaudRateComboBox.Items.Count == 0)
            {
                MessageBox.Show("Brak dostępnych portów COM!", "Błąd");
                return;
            }

            ReceivingMessageTextBox.Text = string.Empty;
            StartButton.IsEnabled = StartMenuItem.IsEnabled = SaveToFileButton.IsEnabled = SaveToFileMenuItem.IsEnabled = ClearTextBlockMenuItem.IsEnabled = false;
            StopButton.IsEnabled = StopMenuItem.IsEnabled = true;

            _isCurrentSessionSaved = false;
            _allMessages = new List<Tuple<DateTime, string>>();
            _allMessagesInline = string.Empty;

            _serialPort = new SerialPort(COMPortsComboBox.SelectedItem.ToString(), int.Parse(BaudRateComboBox.SelectedItem.ToString()));
            _serialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            try
            {
                _serialPort.Open();

                if (_serialPort.IsOpen)
                {
                    _serialPort.Write(START_CMD);
                }
                else
                {
                    MessageBox.Show("Nie udało się nawiązać połączenia!", "Błąd");
                    return;
                }
            }
            catch (IOException ex)
            {
                MessageBox.Show(ex.Message, "Błąd");                
            }
        }

        /// <summary>
        /// Obsługa zdarzenia kliknięcia w przycisk Zatrzymania
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            StartButton.IsEnabled = StartMenuItem.IsEnabled = SaveToFileButton.IsEnabled = SaveToFileMenuItem.IsEnabled = ClearTextBlockMenuItem.IsEnabled = true;
            StopButton.IsEnabled = StopMenuItem.IsEnabled = false;

            if (_serialPort.IsOpen)
            {
                _serialPort.Write(STOP_CMD);
            }
            else
            {
                MessageBox.Show("Połączenie COM z " + _serialPort.PortName + " jest już zamknięte!", "Błąd");
                return;
            }

            _serialPort.Close();
        }

        /// <summary>
        /// Obsługa zdarzenia kliknięcia w przycisk Zapisu danych do pliku
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SaveToFileButton_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog fileDialog = new SaveFileDialog();
            fileDialog.Title = "Wybierz plik do zapisu danych";
            fileDialog.Filter = "Text Files (*.txt)|*.txt";
            fileDialog.OverwritePrompt = true;
            fileDialog.FileName = "_" + DateTime.Today.ToString("dd-MM-yyyy") + ".txt";

            bool? dialogResult = fileDialog.ShowDialog();

            if (dialogResult.HasValue ? dialogResult.Value : false)
            {
                SaveDataToFile _saver = new SaveDataToFile(fileDialog.FileName);
                _isCurrentSessionSaved = _saver.SaveData(_allMessages);
            }
        }

        /// <summary>
        /// Obsługa wyjścia z programu
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        /// <summary>
        /// Obsługa wyjścia z aplikacji przez czerwony X w prawym górnym rogu
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Window_Closing(object sender, CancelEventArgs e)
        {
            if (!_isCurrentSessionSaved && _allMessages.Count > 0)
            {
                MessageBoxResult result = MessageBox.Show("Czy chcesz zapisać dane przed wyłączeniem programu?", "Zapisz dane albo utrać je bezpowrotnie", MessageBoxButton.YesNo);
                if (result == MessageBoxResult.Yes)
                {
                    this.SaveToFileButton_Click(null, null);
                }
            }
        }

        /// <summary>
        /// Zdarzenie przechwytujące dane z portu COM
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort serialPort = (SerialPort)sender;
            string receivedMessage = serialPort.ReadExisting();
            _allMessages.Add(new Tuple<DateTime, string>(DateTime.Now, receivedMessage));
            _allMessagesInline += receivedMessage;
            ReceivingMessageTextBox.Dispatcher.Invoke(new Action(() => ReceivingMessageTextBox.Text = _allMessagesInline));
            if (_scrollToTheEnd)
            {
                ReceivingMessageTextBox.Dispatcher.Invoke(new Action(() => ReceivingMessageTextBox.ScrollToEnd()));
            }
        }
        
        /// <summary>
        /// Obsługa odświeżenia listy portów COM
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RefreshPorts_Click(object sender, RoutedEventArgs e)
        {
            BaudRateComboBox.ItemsSource = _availablePortBaudRates;
            BaudRateComboBox.SelectedItem = "9600";

            try
            {
                _portNames = SerialPort.GetPortNames();
                COMPortsComboBox.ItemsSource = _portNames;
                COMPortsComboBox.SelectedIndex = 0;
            }
            catch (Exception ex)
            {
                ReceivingMessageTextBox.Text += "Nie udało się pobrać listy portów COM!" + Environment.NewLine;
                ReceivingMessageTextBox.Text += ex.Message + Environment.NewLine;
                ReceivingMessageTextBox.Text += ex.Source + Environment.NewLine;
                ReceivingMessageTextBox.Text += ex.StackTrace + Environment.NewLine;
            }
        }

        private void ClearTextBlock_Click(object sender, RoutedEventArgs e)
        {
            _allMessages = new List<Tuple<DateTime, string>>();
            _allMessagesInline = string.Empty;
            ReceivingMessageTextBox.Dispatcher.Invoke(new Action(() => ReceivingMessageTextBox.Text = _allMessagesInline));
        }

        private void ScrollToTheEndCheckBox_Click(object sender, RoutedEventArgs e)
        {
            _scrollToTheEnd = ScrollToTheEndCheckBox.IsChecked.HasValue ? ScrollToTheEndCheckBox.IsChecked.Value : false;
        }
    }
}
