using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.MemoryMappedFiles;

namespace HardSense.MemFile
{
    public class MMFile
    {
        private int mmFileItemMaxLength = 10;

        private List<DataItem> map = new List<DataItem>();
        private int fileLength = 0;
        private MemoryMappedFile mmFile = null;
        
        //private double counter = 4;

        public void InitFile()
        {
            if (fileLength == 0 || map.Count == 0)
                throw new Exception("CreateFile:  No items in map.");
            
            mmFile = MemoryMappedFile.CreateNew("HardSenseMemoryMappedFileForTesting", fileLength);

            foreach (DataItem currDataItem in map)
            {
                currDataItem.mmvAccessor = mmFile.CreateViewAccessor(currDataItem.offset, currDataItem.maxLength);
                //currDataItem.mmvAccessor.Write(0, 1111.11);
                //UpdateKeyWithValue(currDataItem.key, 111.11d);
                //mmFileViewAccessor.Write(currDataItem.offset, counter);
                //counter = counter * 6.8;
            }
            
        }
        
        /*
        public void IterateMap()
        {
            foreach(DataItem currDataItem in map)
            {
                double s = currDataItem.mmvAccessor.ReadDouble(0);
                string x = "interesitng";

                currDataItem.mmvAccessor.Write(0, 3d);

                //double s = mmFileViewAccessor.ReadDouble(currDataItem.offset);
                //string x = "interesitng";
            }
        }
        */
        public void AddNewDataItem(string newKey, int newMaxLength)
        {
            map.Add(new DataItem(newKey, newMaxLength, fileLength));
            fileLength += newMaxLength;
        }

        public void UpdateKeyWithValue(string key, double value)
        {
            DataItem currDataItem = FindItemByKey(key);
            if (currDataItem == null)
                return;

            if(value.ToString().Length > mmFileItemMaxLength)
                value = Convert.ToDouble(value.ToString().Substring(0,mmFileItemMaxLength));

            currDataItem.mmvAccessor.Write(0, value);
        }

        public double GetValueByKey(string key)
        {
            DataItem currDataItem = FindItemByKey(key);
            if (currDataItem == null)
                return 0d;

            return currDataItem.mmvAccessor.ReadDouble(0);
        }

        private DataItem FindItemByKey(string needle)
        {
            if (map.Count == 0)
                throw new Exception("MMFileMap::FindItemByKey() - Map is empty");

            foreach(DataItem currItem in map)
            {
                if (currItem.key.Equals(needle))
                    return currItem;
            }
            return null;
        }

        public void Clear()
        {
            map.Clear();
            fileLength = 0;
            if(mmFile != null)
                mmFile.Dispose();
        }
        
    }

    public class DataItem
    {
        public string key { get; private set; }
        public int maxLength { get; private set; }
        public int offset { get; private set; }
        public MemoryMappedViewAccessor mmvAccessor { get; set; }
        
        public DataItem(string newKey, int newMaxLength, int newOffset)
        {
            key = newKey;
            maxLength = newMaxLength;
            offset = newOffset;
        }
    }
}
