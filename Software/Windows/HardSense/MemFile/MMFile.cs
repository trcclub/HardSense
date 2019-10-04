using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HardSense.MemFile
{
    public class MMFile
    {
        private List<DataItem> map = new List<DataItem>();
        private int fileLength = 0;

        
        /*
        public void AddNewDataItem(string newKey, int newMaxLength)
        {
            map.Add(new DataItem(newKey, newMaxLength, lastOffset));
            lastOffset += newMaxLength;
        }

        public int FindItemOffsetByKey(string needle)
        {
            if (map.Count == 0)
                throw new Exception("MMFileMap::FindItemByKey() - Map is empty");

            foreach(DataItem currItem in map)
            {
                if (currItem.key.Equals(needle))
                    return currItem.offset;
            }
            return -1;
        }
        */
    }

    public class DataItem
    {
        public string key { get; private set; }
        public int maxLength { get; private set; }
        public int offset { get; private set; }

        public DataItem()
        {

        }

        public DataItem(string newKey, int newMaxLength, int newOffset)
        {
            key = newKey;
            maxLength = newMaxLength;
            offset = newOffset;
        }
    }
}
