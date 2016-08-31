using System;
using System.Collections.Generic;
using System.IO;

namespace COMPortLinkApp
{
    /// <summary>
    /// Klasa odpowiedzialna za logikę zapisu danych do pliku
    /// </summary>
    class SaveDataToFile
    {
        /// <summary>
        /// Isntancja klasy StreamWriter - strumień zapisu
        /// </summary>
        private StreamWriter _streamWriter;

        /// <summary>
        /// Konsturktor parametryczny
        /// </summary>
        /// <param name="path">Ścieżka do pliku w którym mają zostać zapisane dane</param>
        public SaveDataToFile(string path)
        {
            _streamWriter = new StreamWriter(path);
        }

        /// <summary>
        /// Metoda zapisująca wszystkie otrzymane dane w pliku i zamykająca strumień
        /// </summary>
        /// <param name="datas"></param>
        /// <returns></returns>
        public bool SaveData(List<Tuple<DateTime, string>> datas)
        {
            try
            {
                foreach (Tuple<DateTime, string> data in datas)
                {
                    _streamWriter.WriteLine(data.Item1.ToString("HH:mm:ss.fff") + " " + data.Item2);
                }

                return true;
            }
            catch
            {
                return false;
            }
            finally
            {
                _streamWriter.Close();
            }
        }
    }
}
