using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SensorDataAnalyserApp
{
    static class HelperComponents
    {
        private const string _tab = "\t";

        public static string DictionaryToString(SortedDictionary<string, uint> Histogram)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("Klucz");
            sb.Append(_tab);
            sb.Append(_tab);
            sb.Append("Wartość");
            sb.Append(Environment.NewLine);
            
            foreach (KeyValuePair<string, uint> pair in Histogram)
            {
                sb.Append(pair.Key);
                sb.Append(_tab);
                sb.Append(_tab);
                sb.Append(pair.Value);
                sb.Append(Environment.NewLine);
            }

            return sb.ToString();
        }
    }
}
