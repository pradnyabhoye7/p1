#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <cstdlib>   // For rand()
#include <algorithm> // For shuffle
#include <ctime>     // For seeding rand()

using namespace std;

class PCB
{
public:
    int JID; // Job ID
    int TTL; // Total Time Limit
    int TLL; // Total Line Limit
    int LLC; // Lines Limit Counter
    int TTC; // Total Time Counter
};

class OS
{
private:
    char M[300][4]; // Memory of 300 words now
    char IR[4];
    int IC;
    char GR[4];
    int SI;
    int TI;
    int PI;
    bool C;
    char buffer[41];
    PCB pcb; // Process Control Block
    int PTR; // page table register
    int RA;  // Real Address
    int EM;  // Error Message
    string Eroor[7] = {"No Error", "Out of Data", "Line Limit Exceeded", "Time Limit Exceeded", "Operation Code Error", "Operand Error", "Invalid Page Fault"};

public:
    void init();
    void LOAD();
    void PRINTMEMORY();
    void StartExe();
    void Start_User_Pg();
    void MOS();
    void Read();
    void Write();
    void Load_register();
    void Store_register();
    void Compare_register();
    void Branch_on_true();
    void Terminate(int a, int b);
    // in Terminate(a,b) a is first error code and b is second error code(B==-1 means no second error code)
    int getAddress();
    int allocate();
    bool isBlockFree(int blockIndex);
    void initPageTable();
    void addressMap(int VA);

    fstream infile;
    fstream outfile;
};

void OS::initPageTable()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[PTR + i][j] = '*';
        }
    }
}

bool OS::isBlockFree(int blockIndex)
{
    int startLine = blockIndex * 10;
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (M[startLine + i][j] != ' ')
            {
                return false;
            }
        }
    }
    return true;
}

// Allocate a random free block
int OS::allocate()
{
    std::vector<int> freeBlocks;

    // Collect all free blocks
    for (int i = 0; i < 30; ++i)
    {
        if (isBlockFree(i))
        {
            freeBlocks.push_back(i);
        }
    }

    if (freeBlocks.empty())
    {
        return -1;
    }


    // Choose a random block from available
    int randIndex = rand() % freeBlocks.size();
    return freeBlocks[randIndex];
}

int OS::getAddress()
{
    int address = (IR[2] - '0') * 10 + (IR[3] - '0');

    return address;
}

void OS::init()
{
    // Initialize all registers and memory
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = ' ';
        }
    }
    for (int j = 0; j < 4; j++)
    {
        IR[j] = ' ';
        GR[j] = ' ';
    }
    pcb.JID = 0;
    pcb.TTL = 0;
    pcb.TLL = 0;
    pcb.LLC = 0;
    pcb.TTC = 0;
    PTR = 0;
    RA = 0;
    IC = 0;
    SI = 0;
    TI = 0;
    PI = 0;
    C = false;
}

void OS::Read()
{
    SI = 0;

    cout << "Read Executed" << endl;
    pcb.TTC += 2; // Increment Total Time Counter by 2 for Read operation

    if (infile.eof())
    {
        cout << "Error: Unexpected EOF while reading." << endl;
        return;
    }

    for (int i = 0; i < 41; i++)
        buffer[i] = '\0';

    infile.getline(buffer, 41);
    cout << "Buffer: " << buffer << endl;

    // No Data Error if direcly $END is encountered
    if (strncmp(buffer, "$END", 4) == 0)
    {
        Terminate(1, -1);
        return;
    }
    if (buffer[0] == '\0')
    {
        cout << "Warning: Empty buffer in Read()." << endl;
        return;
    }

    cout << "Getting Data To Memory Loaction: " << RA << endl;

    if (RA < 0 || RA >= 300)
    {
        cout << "Error: Invalid memory address." << endl;
        return;
    }

    // getting the data from buffer to memory
    int k = 0;

    for (int i = 0; i < 10 && k < 40; i++)
    {
        for (int j = 0; j < 4 && k < 40; j++)
        {
            M[RA][j] = buffer[k++];
        }
        RA++;
    }
}

void OS::Write()
{
    SI = 0;
    cout << "Write Executed" << endl;

    int address = RA;

    cout << "Reading from memory location: " << address << endl;

    if (address < 0 || address >= 300)
    {
        cout << "Error: Invalid memory address." << endl;
        return;
    }
    pcb.TTC++; // Increment Total Time Counter by 1 for Write operation
    pcb.LLC++; // Increment Line Limit Counter by 1 for Write operation

    if (pcb.LLC > pcb.TLL)
    {
        cout << "Error: Line Limit Exceeded." << endl;
        Terminate(2, -1);
        return;
    }

    // Write the data from memory to output file
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (M[address][j] == '\0')
            {
                break;
            }
            outfile << M[address][j];
            cout << M[address][j];
        }
        address++;
    }

    outfile << endl;
    cout << endl;
}

void OS::Load_register()
{
    pcb.TTC++;
    int address = RA;
    memcpy(GR, M[address], 4);
    cout << "Loaded from M[" << address << "] to GR: " << std::string(GR, 4) << endl;
}

void OS::Store_register()
{
    pcb.TTC += 2; // Increment Total Time Counter by 2 for Store operation
    int address = RA;
    memcpy(M[address], GR, 4);
    cout << "Stored from GR to M[" << address << "]: " << M[address][0] << M[address][1] << M[address][2] << M[address][3] << endl;
}

void OS::Compare_register()
{
    pcb.TTC++; // Increment Total Time Counter by 1 for Compare operation
    cout << "Compare Executed" << endl;
    int address = RA;
    C = (memcmp(GR, M[address], 4) == 0);
    cout << "Comparison with M[" << address << "]: " << (C ? "Equal" : "Not Equal") << endl;
}

void OS::Branch_on_true()
{
    pcb.TTC++; // Increment Total Time Counter by 1 for Branch operation
    cout << "Branch on True Executed" << endl;
    int address = RA;
    if (C)
    {
        IC = address;
        cout << "Branching to IC: " << IC << endl;
    }
    else
    {
        cout << "Condition false, continuing execution." << endl;
    }
}

void OS::Terminate(int a, int b)
{
    if (a == 0 && b == -1)
    {
        outfile << endl;
        cout << "Terminated Successfully" << endl;
        outfile << "Program Terminated Normally" << endl;
        // printing status of all registers to output file
        outfile << "Status of Registers: " << endl;
        outfile << "IC: " << IC << ", ";
        outfile << "GR: " << GR[0] << GR[1] << GR[2] << GR[3] << ", ";
        outfile << "SI: " << SI << ", ";
        outfile << "TI: " << TI << ", ";
        outfile << "PI: " << PI << ", ";
        outfile << "TTC: " << pcb.TTC << ", ";
        outfile << "TLL: " << pcb.TLL << ", ";
        outfile << "LLC: " << pcb.LLC << ", ";
        outfile << "JID: " << pcb.JID << ", ";
        outfile << "PTR: " << PTR << ", ";
        outfile << "RA: " << RA << ", ";
        outfile << "C: " << C << ", ";
        outfile << "IR: " << IR[0] << IR[1] << IR[2] << IR[3] << " ";
        outfile << endl
                << endl
                << endl;
        return;
    }
    else
    {
        cout << "Program Terminated Abnormally" << endl;
        outfile << "Program Terminated Abnormally ";
        if (b == -1)
        {
            cout << "with Error: " << Eroor[a] << endl;
            outfile << "with Error: " << Eroor[a] << endl;
            outfile << "Status of Registers: " << endl;
            outfile << "IC: " << IC << ", ";
            outfile << "GR: " << GR[0] << GR[1] << GR[2] << GR[3] << ", ";
            outfile << "SI: " << SI << ", ";
            outfile << "TI: " << TI << ", ";
            outfile << "PI: " << PI << ", ";
            outfile << "TTC: " << pcb.TTC << ", ";
            outfile << "TLL: " << pcb.TLL << ", ";
            outfile << "LLC: " << pcb.LLC << ", ";
            outfile << "JID: " << pcb.JID << ", ";
            outfile << "PTR: " << PTR << ", ";
            outfile << "RA: " << RA << ", ";
            outfile << "C: " << C << ", ";
            outfile << "IR: " << IR[0] << IR[1] << IR[2] << IR[3] << " ";
            outfile << endl
                    << endl
                    << endl;
            ;
            return;
        }
        else
        {
            cout << "with Errors: " << Eroor[a] << " and " << Eroor[b] << endl;
            outfile << "with Errors: " << Eroor[a] << " and " << Eroor[b] << endl;
            outfile << "Status of Registers: " << endl;
            outfile << "IC: " << IC << ", ";
            outfile << "GR: " << GR[0] << GR[1] << GR[2] << GR[3] << ", ";
            outfile << "SI: " << SI << ", ";
            outfile << "TI: " << TI << ", ";
            outfile << "PI: " << PI << ", ";
            outfile << "TTC: " << pcb.TTC << ", ";
            outfile << "TLL: " << pcb.TLL << ", ";
            outfile << "LLC: " << pcb.LLC << ", ";
            outfile << "JID: " << pcb.JID << ", ";
            outfile << "PTR: " << PTR << ", ";
            outfile << "RA: " << RA << ", ";
            outfile << "C: " << C << ", ";
            outfile << "IR: " << IR[0] << IR[1] << IR[2] << IR[3] << " ";
            outfile << endl
                    << endl
                    << endl;

            return;
        }
        // cout << "Terminated Successfully" << endl;
        // printing status of all registers to output file
    }
}

void OS::MOS()
{
    if (SI == 1)
    {
        if (TI == 0 && PI == 0)
        {
            Read();
            SI = 0; // Reset Service Interrupt
            return;
        }
        else if (TI == 2 && PI == 0)
        {
            Terminate(3, -1);
            SI = 0; // Reset Service Interrupt
            return;
        }
    }
    else if (SI == 2)
    {
        if (TI == 0 && PI == 0)
        {
            Write();
            SI = 0; // Reset Service Interrupt
            return;
        }
        else if (TI == 2 && PI == 0)
        {
            Write();
            SI = 0; // Reset Service Interrupt
            Terminate(3, -1);
            return;
        }
    }
    else if (SI == 3)
    {
        if ((TI == 0 || TI == 2) && PI == 0)
        {
            Terminate(0, -1);
            SI = 0; // Reset Service Interrupt
            return;
        }
    }

    if (PI == 1)
    {
        if (TI == 0)
        {
            Terminate(4, -1);
            return;
        }
        else if (TI == 2)
        {
            Terminate(3, 4);
            return;
        }
    }
    else if (PI == 2)
    {
        if (TI == 0)
        {
            Terminate(5, -1);
            return;
        }
        else if (TI == 2)
        {
            Terminate(3, 5);
            return;
        }
    }
    else if (PI == 3)
    {
        if (TI == 0)
        {
            //  If Page Fault is Valid -> ALLOCATE -> update page Table-> Adjust IC if necessary,
            //  EXECUTE USER PROGRAM OTHERWISE TERMINATE (6)

            RA = getAddress();

            cout << "real address: " << RA << endl;

            if (((IR[0] == 'G' && IR[1] == 'D') || IR[0] == 'S' && IR[1] == 'R') && M[PTR + (RA / 10)][0] == '*')
            {
                cout << "VALID Page Fault at IC: " << IC << endl;

                int newBlock = allocate(); // Allocate a new block for DATA
                cout << "Allocated new block for DATA: " << newBlock << endl;

                M[PTR + (RA / 10)][0] = '0';                   // Mark the page as valid
                M[PTR + (RA / 10)][1] = '0';                   // Mark the page as valid
                M[PTR + (RA / 10)][2] = (newBlock / 10) + '0'; // Store the block number in the page table
                M[PTR + (RA / 10)][3] = (newBlock % 10) + '0';

                addressMap(getAddress());
                PI = 0; // Reset Page Fault Interrupt
                return;
            }
            else
            {
                cout << "Invalid Page Fault" << endl;
                Terminate(6, -1);
                return;
            }
        }
        else if (TI == 2)
        {
            Terminate(3, -1);
            return;
        }
    }

    if (PI == 0 && SI == 0 && TI == 2)
    {
        cout << "Time Limit Exceeded" << endl;
        Terminate(3, -1);
        return;
    }
}

void OS::addressMap(int VA)
{
    int pageNumber = VA / 10; // Calculate the page number

    int offset = VA % 10;

    // Check if the page is Present in the page table
    if (M[PTR + pageNumber][0] == '*')
    {
        cout << "Page Fault Occured" << endl; // means page is not present in the page table
        PI = 3;                               // Set Page Fault Interrupt
        MOS();                                // Call the MOS function to handle the pagefault/terminate
    }

    // Return if page fault is invalid
    if (PI == 3)
    {
        return;
    }

    // PTE refers to the block number stored in that page frame
    int PTE;

    PTE = (M[PTR + pageNumber][2] - '0') * 10 + (M[PTR + pageNumber][3] - '0'); // char to int
    RA = PTE * 10 + offset; // Combine with offset
    cout << "Virtual Address: " << VA << ", Page Number: " << pageNumber << ", Offset: " << offset << ", Real Address: " << RA << endl;
}

void OS::Start_User_Pg()
{

    while (IC < 100)
    {

        addressMap(IC); // Map the IC to the real address

        // Check if memory loaction is blank
        if (M[RA][0] == ' ' && M[RA][1] == ' ' && M[RA][2] == ' ' && M[RA][3] == ' ')
        {
            cout << "Blank Instruction Moving Ahead" << endl;
            IC++;
            continue;
        }

        memcpy(IR, M[RA], 4);
        cout << "Executing Instruction: " << IR[0] << IR[1] << " at IC: " << IC << endl;

        // Check for valid instruction
        if (!((IR[0] == 'G' && IR[1] == 'D') || (IR[0] == 'P' && IR[1] == 'D') || (IR[0] == 'L' && IR[1] == 'R') || (IR[0] == 'S' && IR[1] == 'R') || (IR[0] == 'C' && IR[1] == 'R') || (IR[0] == 'B' && IR[1] == 'T') || IR[0] == 'H'))
        {
            cout << "Stopping execution at IC: " << IC << " (No valid instruction found)" << endl;
            PI = 1; // Set Operation Code Error Interrupt
            MOS();  // Call the MOS function to handle the error/terminate
            return;
        }

        // Check for operand error
        if (IR[0] != 'H') // as H don't have any oprand
        {
        IC++;
        addressMap(IC); // Map the IC to the real address
        cout << "second time address mapped for IC to check operand error" << endl;

        memcpy(IR, M[RA], 4);
        cout << "third time address mapped for IC to regain the IR" << endl;
        // check if IR[0] is integer if yes then set PI=2
        if (IR[0] >= '0' && IR[0] <= '9')
        {
            PI = 2; // Set Operand Error Interrupt
            MOS();  // Call the MOS function to handle the error/terminate
            return;
        }
        IC--;  //in TI=0 && PI=3 ->adjust IC if necessary - while handling page fault we come to MOS and 1 unit time is already over but the instruction is not executed and then when we go in execute user_pg it will point to the next instruction but the previous instruction (GD) is not yet executed that's why we have to decrement IC so that we can fetch same GD instruction to execute now. SO we are first time handling the page fault for 1 unit time and in the 2nd unit we are actually executing the GD 
        addressMap(IC);
        memcpy(IR, M[RA], 4);

        
            int oprand = getAddress(); // Get the operand address
            addressMap(oprand);        // Map the address(inside instruction) after instruction fetch
        }

        // Check for any PI interupt as we check for invalid page fault above
        if (PI != 0)
        {
            return;
        }

        // Increment Instruction Counter
        IC++;

        // Execute the instructions
        if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }
        else if (IR[0] == 'L' && IR[1] == 'R')
        {
            Load_register();
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            Store_register();
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            Compare_register();
        }
        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            Branch_on_true();
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS();
            return;
        }

        // Check for any Time Limit Exceeded or Line Limit Exceeded
        if (pcb.TTC > pcb.TTL)
        {
            TI = 2; // Set Time Limit Exceeded Interrupt
            MOS();
            return;
        }
        if (pcb.LLC > pcb.TLL)
        {
            return;
        }

        // after returning from Read when there is no data in the buffer below we returned from here as well
        if (strncmp(buffer, "$END", 4) == 0)
        {
            return;
        }

        cout << endl
             << endl;
        cout << "Current Time Counter: " << pcb.TTC << endl
             << endl;
    }
}

void OS::StartExe()
{
    IC = 0;
    Start_User_Pg();
}

void OS::LOAD()
{
    int memPtr = 0;        // Persistent memory index
    bool loadMode = false; // Only true between $AMJ and $DTA
    int cardNo = 0;        // Card Number
    cout << "Reading Input File..." << endl; 
    while (true)
    {
        for (int i = 0; i < 41; i++)
            buffer[i] = '\0';

        if (!infile.getline(buffer, 41))
            break;

        if (strncmp(buffer, "$AMJ", 4) == 0)
        {
            init();

            // Extract values from buffer
            std::string jobStr(buffer);
            std::string jidStr = jobStr.substr(4, 4);  // Characters 4-7
            std::string ttlStr = jobStr.substr(8, 4);  // Characters 8-11
            std::string tllStr = jobStr.substr(12, 4); // Characters 12-15

            // Convert to integers
            pcb.JID = std::stoi(jidStr);
            pcb.TTL = std::stoi(ttlStr);
            pcb.TLL = std::stoi(tllStr);

            pcb.LLC = 0; // Initialize Line Limit Counter
            pcb.TTC = 0; // Initialize Total Time Counter

            cout << "Job ID: " << pcb.JID << ", TTL: " << pcb.TTL << ", TLL: " << pcb.TLL << endl;
            PTR = 10 * allocate(); // Allocate a block of memory for page table
            initPageTable();       // Initialize page table
            cout << "Allocated Page Table Block: " << PTR / 10 << endl;
            cardNo = PTR;
            memPtr = 0;      // Reset for new program
            loadMode = true; // Start accepting instructions
        }
        else if (strncmp(buffer, "$DTA", 4) == 0)
        {
            loadMode = false; // Stop loading instructions
            StartExe();       // Start execution
        }
        else if (strncmp(buffer, "$END", 4) == 0)
        {
            continue;
        }
        else if (loadMode)
        {
            memPtr = allocate(); // Allocate a block of memory for the job
            cout << "Allocated Memory Block: " << memPtr << endl;
            M[cardNo][0] = '0';
            M[cardNo][1] = '0';
            M[cardNo][2] = (memPtr / 10) + '0';
            M[cardNo][3] = (memPtr % 10) + '0';
            cardNo++;
            if (cardNo == 10 + PTR)
            {
                cardNo = 0;
            }
            int k = 0;
            memPtr = memPtr * 10; // Calculate the starting address in memory
            while (buffer[k] != '\0' && memPtr < 300 && k < 40)
            {
                for (int j = 0; j < 4 && k < 40; j++)
                {
                    if (buffer[k] == '\0')
                        break;

                    M[memPtr][j] = buffer[k];

                    if (buffer[k] == 'H') // Stop after halt
                    {
                        j = 3;
                        // break;
                    }

                    k++;
                }

                memPtr++;
            }
        }
    }
}

void OS::PRINTMEMORY()
{
    cout << "\nMemory Dump:\n";
    for (int i = 0; i < 300; i++)
    {
        cout << "[" << i << "] ";
        for (int j = 0; j < 4; j++)
        {
            cout << M[i][j];
        }
        cout << endl;
    }
}

int main()
{
    OS os;
    srand(time(nullptr)); // Seed once at program start (its for randomly choosing any number out of 30)

    os.infile.open("input.txt", ios::in);
    os.outfile.open("output.txt", ios::out);

    if (!os.infile)
    {
        cout << "Failure: File not found!" << endl;
        return 1;
    }

    os.LOAD();
    os.PRINTMEMORY();

    os.infile.close();
    os.outfile.close();

    return 0;
} 