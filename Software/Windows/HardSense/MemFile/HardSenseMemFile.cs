using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.IO.MemoryMappedFiles;

namespace HardSense.MemFile
{
    public class HardSenseMemFile
    {
        private static bool isCreator = false;
        private int mmFileItemMaxLength = 10;

        private static List<DataItem> map = new List<DataItem>();
        private int fileLength = 0;
        private static MemoryMappedFile mmFile = null;
        
        //private double counter = 4;
        public HardSenseMemFile(bool creatorRequest = false)
        {
            if (creatorRequest)
            {
                if (isCreator)
                {
                    throw new Exception("HardSenseMemFile():  A HardSenseMemFile creator already exists.");
                }
                isCreator = true;
            }
        }



        public void InitializeMemoryMappedFileWithData()
        {
            if (!isCreator)
            {
                throw new Exception("HardSenseMemFile::InitializeMemoryMappedFileWithData():  Must be a creator to access.");
            }

            if (fileLength == 0 || map.Count == 0)
            {
                throw new Exception("HardSenseMemFile::InitializeMemoryMappedFileWithData():  No items in map.");
            }
            
            mmFile = MemoryMappedFile.CreateNew(Properties.Settings.Default.MemoryMapFileName, fileLength);

            foreach (DataItem currDataItem in map)
            {
                currDataItem.SetAccessor(mmFile.CreateViewAccessor(currDataItem.offset, currDataItem.maxLength));
            }
            
        }
        
        public void AddNewDataItem(string newKey, int newMaxLength)
        {
            if (!isCreator)
            {
                throw new Exception("HardSenseMemFile::AddNewDataItem():  Must be a creator to access.");
            }

            map.Add(new DataItem(newKey, newMaxLength, fileLength));
            fileLength += newMaxLength;
        }

        public void UpdateKeyWithValue(string key, double value)
        {
            if (!isCreator)
            {
                throw new Exception("HardSenseMemFile::UpdateKeyWithValue():  Must be a creator to access.");
            }

            DataItem currDataItem = FindItemByKey(key);
            if (currDataItem == null)
                return;

            if(value.ToString().Length > mmFileItemMaxLength)
                value = Convert.ToDouble(value.ToString().Substring(0,mmFileItemMaxLength));

            currDataItem.WriteDouble(value);
        }

        public static bool DoesIdExistInList(string needle)
        {
            foreach (DataItem currItem in map)
            {
                if (currItem.key.Equals(needle))
                    return true;
            }
            return false;

        }

        public static double GetValueByKey(string key)
        {
            DataItem currDataItem = FindItemByKey(key);
            if (currDataItem == null)
                return 0d;

            return currDataItem.ReadDouble();
        }

        private static DataItem FindItemByKey(string needle)
        {
            if (map.Count == 0)
                throw new Exception("HardSenseMemFile::FindItemByKey() - Map is empty");

            foreach(DataItem currItem in map)
            {
                if (currItem.key.Equals(needle))
                    return currItem;
            }
            return null;
        }

        public void Clear()
        {
            if (!isCreator)
            {
                throw new Exception("HardSenseMemFile::Clear():  Must be a creator to access.");
            }

            map.Clear();
            fileLength = 0;
            if(mmFile != null)
                mmFile.Dispose();
        }
        
    }

    public class DataItem
    {
        public string key { get; private set; } = "";
        public int maxLength { get; private set; } = 0;
        public int offset { get; private set; } = 0;

        private bool ready = false;
        private MemoryMappedViewAccessor mmvAccessor = null;
        private Mutex accessorMutex = new Mutex();

        public DataItem(string newKey, int newMaxLength, int newOffset)
        {
            key = newKey;
            maxLength = newMaxLength;
            offset = newOffset;
        }

        public void SetAccessor(MemoryMappedViewAccessor newAccessor)
        {
            mmvAccessor = newAccessor;
            ready = true;
        }

        public void WriteDouble(double value)
        {
            if (ready)
            {
                accessorMutex.WaitOne();
                mmvAccessor.Write(0, value);
                accessorMutex.ReleaseMutex();
            }
        }

        public double ReadDouble()
        {
            double ret = 0;
            if (ready)
            {
                accessorMutex.WaitOne();
                ret = mmvAccessor.ReadDouble(0);
                accessorMutex.ReleaseMutex();
            }
            return ret;
        }
    }
}
