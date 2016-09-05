using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SensorDataAnalyserApp
{
    class FileCheckerAndLoader
    {
        public string[] ListOfFiles { get; private set; }

        public FileCheckerAndLoader(string[] ListOfFiles)
        {
            this.ListOfFiles = ListOfFiles;
        }

        public bool IsAllFileFromTheSameSensor()
        {
            List<string> _sensorName = new List<string>();

            foreach (string _path in ListOfFiles)
            {
                _sensorName.Add(Path.GetFileName(_path));
            }

            if (_sensorName.Count > 0)
            {
                string firstValue = _sensorName.First().Substring(0, _sensorName.First().IndexOf("_"));
                return _sensorName.All(name => name.Substring(0, name.IndexOf("_")) == firstValue);
            }
            else
            {
                return false;
            }
        }

        private SortedDictionary<string, uint> loadDataAndComputeHistogram(int digits)
        {
            SortedDictionary<string, uint> histogram = new SortedDictionary<string, uint>();

            foreach (string _path in ListOfFiles)
            {
                using (StreamReader _streamReader = new StreamReader(_path))
                {
                    List<string> _datasFromFile = new List<string>();

                    _streamReader.ReadLine(); // Pominięcie pierwszej linii pliku - nazwa czujnika
                    _streamReader.ReadLine(); // Pominięcie drugiej linii pliku - jednostka

                    while (_streamReader.Peek() >= 0)
                    {
                        string _temp = _streamReader.ReadLine();

                        if (_temp == Environment.NewLine || _temp == string.Empty) { }
                        else
                        {
                            _datasFromFile.Add(_temp);
                        }
                    }
                    
                    double _previousValue = double.Parse(_datasFromFile.First().Substring(_datasFromFile.First().IndexOf(" ")), CultureInfo.InvariantCulture);

                    for (int i = 1; i < _datasFromFile.Count; i++)
                    {
                        string _data = _datasFromFile[i];
                        double _currentValue = double.Parse(_data.Substring(_data.IndexOf(" ")), CultureInfo.InvariantCulture);
                        double _difference = Math.Abs(_currentValue - _previousValue);
                        string _key = _difference.ToString("N" + digits);
                        _previousValue = _currentValue;

                        if (histogram.ContainsKey(_key))
                        {
                            histogram[_key]++;
                        }
                        else
                        {
                            histogram[_key] = 1;
                        }
                    }
                }
            }

            return consolidationOfNearestValues(histogram, digits);
        }


        private bool checkAllFiles()
        {
            foreach (string _path in ListOfFiles)
            {
                if (File.Exists(_path)) { }
                else
                {
                    return false;
                }
            }

            return true;
        }

        public SortedDictionary<string, uint> LoadDataAndComputeHistogram(int digits)
        {
            SortedDictionary<string, uint> histogram = new SortedDictionary<string, uint>();

            if (checkAllFiles())
            {
                return loadDataAndComputeHistogram(digits);
            }
            else
            {
                return null;
            }
        }

        private SortedDictionary<string, uint> consolidationOfNearestValues(SortedDictionary<string, uint> unconsolidatedHistogram, int digits)
        {
            SortedDictionary<string, uint> consolidatedHistogram = new SortedDictionary<string, uint>();

            string _previousKey = unconsolidatedHistogram.Keys.First();
            double _previousKeyValue = double.Parse(_previousKey);

            foreach (KeyValuePair<string, uint> pair in unconsolidatedHistogram)
            {
                double _currentKeyValue = double.Parse(pair.Key);
                double _differenceBetweenKeys = Math.Abs(_previousKeyValue - _currentKeyValue);
                _previousKeyValue = _currentKeyValue;

                if (_differenceBetweenKeys == 0d)
                {
                    consolidatedHistogram.Add(pair.Key, pair.Value);
                }
                else if (_differenceBetweenKeys <= 2 * Math.Pow(10, -digits) && _differenceBetweenKeys > 0)
                {
                    consolidatedHistogram[_previousKey] += pair.Value;
                }
                else
                {
                    consolidatedHistogram.Add(pair.Key, pair.Value);
                    _previousKey = pair.Key;
                }
            }

            return consolidatedHistogram;
        }
    }
}
