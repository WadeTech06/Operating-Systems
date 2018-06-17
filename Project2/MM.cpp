#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <list>

using namespace std;
class Segment
{
  public:
    //Data Members
    int byte_size;
    int text_size;
    int data_size;
    int heap_size;
    int pid;
    int *text_segment;
    int *data_segment;
    int *heap_segment;

    Segment()
    {
    }

    Segment(int sBytes, int sPid, int sText, int sData, int sHeap)
    {
        if (sBytes != (sText + sData + sHeap))
            byte_size = -1;
        else
        {
            byte_size = sBytes;
            pid = sPid;
            text_segment = new int[sText];
            data_segment = new int[sData];
            heap_segment = new int[sHeap];

            text_size = sText;
            data_size = sData;
            heap_size = sHeap;
        }
    }
};
class Page
{
  public:
    int byte_size;
    int total_size;
    int *page;
    int pid;

    Page()
    {
    }
    Page(int sBytes, int sPid, int sTotalSize)
    {

        byte_size = sBytes;
        total_size = sTotalSize;
        page = new int[sBytes];
        pid = sPid;
    }
};

class MemoryManager
{
  private:
    //segment variables
    list<Segment> _segmentList;
    int _allocatedBlocks;
    int _freeBlocks;

    // page variables
    list<Page> _pageList;
    int _TotalProcess;
    int _freePages;
    int _allocatedPages;

    //  mixed variables
    int _memorySize;
    int _failedAllocation;
    int _deletedAllocations;
    int _policy;

  public:
    MemoryManager(int bytes, int policy)
    {
        // intialize memory with these many bytes.
        _memorySize = bytes;
        _allocatedBlocks = 0;
        _freeBlocks = _memorySize;
        _policy = policy;
        _failedAllocation = 0;

        _allocatedPages = 0;
        _freePages = GetNumberOfPages(_memorySize);
    }
    int allocate(int bytes, int pid, int text_size, int data_size, int heap_size)
    {
        if (_policy == 0)
            return Segmentation(bytes, pid, text_size, data_size, heap_size);
        else if (_policy == 1)
            return Paging(bytes, pid);
        else
            cout << "policy not found\n";
        return -1;
    }

    int deallocate(int pid)
    { //deallocate memory allocated to this process
        // return 1 if successful, -1 otherwise with an error message
        int byteSize = 0;
        if (_policy == 0)
        {
            //FIRST-FIT
            _segmentList.erase(
                std::remove_if(_segmentList.begin(), _segmentList.end(),
                               [&pid, &byteSize](const Segment &s) { byteSize = s.byte_size; return (s.pid == pid); }),
                _segmentList.end());
            UpdateMemory(byteSize, 1);
            _deletedAllocations++;
            return 1;
        }
        else if (_policy == 1)
        {
            _pageList.erase(
                std::remove_if(_pageList.begin(), _pageList.end(),
                               [&pid, &byteSize](const Page &p) { if(pid == p.pid){byteSize = p.total_size;}  return (p.pid == pid); }),
                _pageList.end());
            UpdateMemory(byteSize, 1);
            _deletedAllocations++;
            _TotalProcess--;
            _pageList.sort([](Page lhs, Page rhs) { return lhs.pid < rhs.pid; });
            return 1;
        }
        else
            cout << "policy not found";
        return -1;
    }

    void PrintMemoryState()
    {
        // print out current state of memory
        if (_policy == 0)
        {
            cout << "Memory size = " << _memorySize << " bytes, "
                 << "Allocated = " << _allocatedBlocks << " bytes, "
                 << "Free = " << _freeBlocks << " bytes\n";

            cout << "There are currently " << _segmentList.size() << " active process\n";
            cout << "Process list:\n";

            list<Segment>::iterator it;
            for (it = _segmentList.begin(); it != _segmentList.end(); ++it)
            {
                cout << "Proccess id = " << it->pid << " Size = " << it->byte_size;
                cout << "\n text start = " << it->text_segment << ", size = " << it->text_size;
                cout << "\n data start = " << it->data_segment << ", size = " << it->data_size;
                cout << "\n heap start = " << it->heap_segment << ", size = " << it->heap_size;
                cout << "\n";
                cout << "\n";
            }
        }
        else if (_policy == 1)
        {
            cout << "Memory size = " << _memorySize << ", total pages = " << GetNumberOfPages(_memorySize);
            cout << "\nallocated pages = " << _allocatedPages << ", free pages = " << _freePages;
            cout << "\nThere are currently " << _TotalProcess << " active process\n";
            cout << "Process list:\n";

            list<Page>::iterator it;
            list<Page>::iterator itProcess = _pageList.begin();
            int virtualPage = 0;
            for (it = _pageList.begin(); it != _pageList.end(); it++)
            {
                cout << "Process id=" << it->pid << ", size=" << it->total_size << "bytes,";
                cout << " number of pages=" << GetNumberOfPages(it->total_size);
                cout << "\n";
                do
                {
                    cout << "Virt Page " << virtualPage << " -> Phys Page " << it->page << " used: "
                         << it->byte_size << " bytes\n";
                    virtualPage++;
                    it++;
                } while (it->pid == itProcess->pid);
                itProcess = it;
                it--;
            }
        }
        else
            cout << "Policy not found\n";

        cout << "\n";
        cout << "Failed allocations (No memory) = " << _failedAllocation;
        cout << "\nDeleted allocations = " << _deletedAllocations;
        cout << "\n";
    }

  private:
    int Segmentation(int bytes, int pid, int text_size, int data_size, int heap_size)
    {
        Segment *s = new Segment(bytes, pid, text_size, data_size, heap_size);

        if (s->byte_size == -1)
        {
            cout << "Failed Allocation";
            _failedAllocation++;
            return -1;
        }
        else
        {
            if (_freeBlocks > s->byte_size)
            {
                _segmentList.push_back(*s);
                UpdateMemory(s->byte_size, 0);
                return 1;
            }
            else
            {
                _failedAllocation++;
                cout << "allocated memory size is smaller than request: " << pid << "\n";
            }
            return -1;
        }
    }
    int Paging(int bytes, int pid)
    {
        _TotalProcess++;

        int totalBytes = _freeBlocks - bytes;
        int allocatingBytes;
        if (totalBytes > 0)
            allocatingBytes = bytes;
        else
        {
            allocatingBytes = abs(totalBytes);
            _failedAllocation++;
        }

        //bytes = a(x)+b paging algorithm
        int a = allocatingBytes / 32;
        int z = a * 32;
        int b = allocatingBytes - z;

        for (int x = 0; x < a; x++)
        {
            Page *p = new Page(32, pid, allocatingBytes);
            if (_freeBlocks > p->byte_size)
            {
                _pageList.push_back(*p);
                UpdateMemory(p->byte_size, 0);
            }
            else
            {
                _failedAllocation++;
                cout << "allocated memory size is smaller than request for pid: " << p->pid << "\n";
                return -1;
            }
        }
        if (b > 0)
        {
            Page *p = new Page(b, pid, allocatingBytes);
            if (_freeBlocks > p->byte_size)
            {
                _pageList.push_back(*p);
                UpdateMemory(p->byte_size, 0);
                _pageList.sort([](Page lhs, Page rhs) { return lhs.pid < rhs.pid; });
                return 1;
            }
            else
            {
                _failedAllocation++;
                cout << "allocated memory size is smaller than request: " << pid << "\n";
                return -1;
            }
        }

        return -1;
    }

    int GetNumberOfPages(int bytes)
    {
        //bytes = a(x)+b paging algorithm
        int a = bytes / 32;
        int z = a * 32;
        int b = bytes - z;

        if (b > 0)
            return a + 1;
        else
            return a;
    }

    void UpdateMemory(int byteSize, int oper)
    {
        // 0 == allocation
        // 1 == deallocation
        if (oper == 0)
        {
            _freeBlocks = _freeBlocks - byteSize;
            _allocatedBlocks = _allocatedBlocks + byteSize;
            if (_policy == 1)
            {
                --_freePages;
                ++_allocatedPages;
            }
        }
        else
        {
            _freeBlocks = _freeBlocks + byteSize;
            _allocatedBlocks = _allocatedBlocks - byteSize;
            if (_policy == 1)
            {
                _freePages = _freePages + (byteSize / 32);
                _allocatedPages = GetNumberOfPages(_memorySize) - _freePages;
            }
        }
    }
};

class MMFileModel
{
    // Access specifier
  public:
    // Data Members
    int byte_size;
    int pid;
    int text_size;
    int data_size;
    int heap_size;

    // Member Functions()
    void print()
    {
        cout << "bytes size: " << byte_size
             << "\npid:" << pid
             << "\ntext_size:" << text_size
             << "\ndata_size:" << data_size
             << "\nheap_size:" << heap_size;
    }
};

int main(int argc, char *argv[])
{
    string line;
    ifstream file(argv[1]);

    MMFileModel mmfileModel;
    string instruction;

    if (file.is_open())
    {
        getline(file, line);
        size_t pos = line.find(" ");
        int byte_size = stoi(line.substr(0, pos));

        line = line.substr(pos + 1);
        pos = line.find(" ");
        int policy = stoi(line.substr(0, pos));

        MemoryManager *mm = new MemoryManager(byte_size, policy);

        while (getline(file, line))
        {
            pos = line.find(" ");
            instruction = line.substr(0, pos);
            line = line.substr(pos + 1);

            if (instruction == "A")
            {
                pos = line.find(" ");
                mmfileModel.byte_size = stoi(line.substr(0, pos));
                line = line.substr(pos + 1);

                pos = line.find(" ");
                mmfileModel.pid = stoi(line.substr(0, pos));
                line = line.substr(pos + 1);

                pos = line.find(" ");
                mmfileModel.text_size = stoi(line.substr(0, pos));
                line = line.substr(pos + 1);

                pos = line.find(" ");
                mmfileModel.data_size = stoi(line.substr(0, pos));
                line = line.substr(pos + 1);

                pos = line.find(" ");
                mmfileModel.heap_size = stoi(line.substr(0, pos));
                line = line.substr(pos + 1);

                mm->allocate(mmfileModel.byte_size,
                             mmfileModel.pid,
                             mmfileModel.text_size,
                             mmfileModel.data_size,
                             mmfileModel.heap_size);
            }
            else if (instruction == "D")
            {
                pos = line.find(" ");
                mmfileModel.pid = stoi(line.substr(0, pos));
                line = line.substr(pos + 1);

                int status = mm->deallocate(mmfileModel.pid);
                if (status == -1)
                {
                    cout << "Error: pid not found\n";
                }
            }
            else if (instruction == "P")
            {
                mm->PrintMemoryState();
            }
        }
    }
    else
    {
        cout << "File not found\n";
    }
}