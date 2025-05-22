#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class OS
{
private:
    char M[100][4]; 
    char IR[4];     
    int IC;         
    char GR[4];        
    int SI;        
    bool C;        
    char buffer[41]; 
    // int returnAddress = -1; 
public:
    void init();
    void LOAD();
    void PRINTMEMORY();
    void StartExe();
    void Start_User_Pg();
    void MOS(int SI);
    void Read();
    void Write();
    void Load_register(); 
    void Store_register();
    void Compare_register();
    void Branch_on_true();
    void Terminate();
    // void Halt();
    int getAddress();

    fstream infile;
    fstream outfile;
};

int OS::getAddress(){
    return (IR[2] - '0') * 10 + (IR[3] - '0');
}

void OS::init()
{
    for (int i = 0; i < 100; i++){
    for (int j = 0; j < 4; j++){
        M[i][j] = ' ';
    }
}

for (int j = 0; j < 4; j++){
    IR[j] = ' ';
    GR[j]= ' ';
}

    IC = 0;
    SI = 0;
    C = false;
}

void OS::Read()
{
    cout << "Read Executed" << endl;

    if (infile.eof()) {
        cout << "Error: Unexpected EOF while reading." << endl;
        return;
    }
    for (int i = 0; i < 40; i++)
    buffer[i] = '\0';

    infile.getline(buffer, 40);
    cout << "Buffer: " << buffer << endl;

    if (buffer[0] == '\0') {
        cout << "Warning: Empty buffer in Read()." << endl;
        return;
    }

    // int here = 0;
    // if (IC - 1 < 0) {
    //     cout << "Error: Invalid IC value in Read()." << endl;
    //     return;
    // }
    // if (IC == 0) {
    //     cout << "Error: Read() called before IC is set correctly." << endl;
    //     return;
    // }

    int address=getAddress();

    cout << "Reading from memory location: " << address << endl;

    if (address < 0 || address >= 100) {
        cout << "Error: Invalid memory address." << endl;
        return;
    }

    int k = 0;
    if (strncmp(buffer, "$END", 4) == 0)
        {
            return;
        }

    for (int i = 0; i < 10 && k < 40; i++) {
        for (int j = 0; j < 4 && k < 40; j++) {
            M[address][j] = buffer[k++];
        }
        address++;
    }
}

void OS::Write()
{
    cout << "Write Executed" << endl;
    
    // if (IC - 1 < 0) {
    //     cout << "Error: Invalid IC value in Write()." << endl;
    //     return;
    // }

    int address=getAddress();

    cout << "Reading from memory location: " << address << endl;

    if (address < 0 || address >= 100) {
        cout << "Error: Invalid memory address." << endl;
        return;
    }

  
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (M[address][j] == '\0') 
        {
            break;
        }
            outfile << M[address][j];
            cout << M[address][j]; //debug print to terminal what we are storing
        }
        address++;
        
    }

    outfile << endl;
    cout << endl;
}

void OS::Load_register() {
    int address = getAddress();
    memcpy(GR, M[address], 4); 
    cout << "Loaded from M[" << address << "] to GR: " << std::string(GR, 4) << endl;
}



void OS::Store_register() {
    int address = getAddress();
    memcpy(M[address], GR, 4);
    cout << "Stored from GR to M[" << address << "]: " << M[address][0] << M[address][1] << M[address][2] << M[address][3] << endl;
}

void OS::Compare_register() {
    int address = getAddress();
    C = (memcmp(GR, M[address], 4) == 0);
    cout << "Comparison with M[" << address << "]: " << (C ? "Equal" : "Not Equal") << endl;
}


void OS::Branch_on_true() {
    int address = getAddress();
    if (C) {
        // returnAddress = IC; 
        IC = address; 
        cout << "Branching to IC: " << IC << endl;
    } else {
        cout << "Condition false, continuing execution." << endl;
    }
}

// void OS::Halt() { 
//     if (returnAddress != -1) {
//         IC = returnAddress; 
//         returnAddress = -1; 
//         cout << "Returning to IC: " << IC << " after branch execution." << endl;
//     } else {
//         cout << "Stopping execution at IC: " << IC << " (No valid instruction found)" << endl;
//     }
// }




void OS::Terminate()
{
    cout<< endl;
    cout << "Terminated Successfully" << endl;
    outfile<<endl<<endl;
    return;
}

void OS::MOS(int SI)
{
    switch (SI)
    {
    case 1:
        Read();
        break;
    case 2:
        Write();
        break;
    case 3:
        Terminate();
        break;
        return;
    }
}

void OS::Start_User_Pg()
{
    if (IC < 0 || IC >= 100) {
        cout << "Error: IC out of bounds! IC=" << IC << endl;
        return;
    }
    while (IC < 100)
    {
        
        memcpy(IR, M[IC], 4);
        
        
        if (!(IR[0] == 'G' || IR[0] == 'P' || IR[0] == 'L' || IR[0] == 'S' || 
            IR[0] == 'C' || IR[0] == 'B' || IR[0] == 'H')) {
          cout << "Stopping execution at IC: " << IC << " (No valid instruction found)" << endl;
          break;
      }

        cout << "Executing Instruction: " << IR[0] << IR[1] << " at IC: " << IC << endl;
        IC++;

        if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS(SI);
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            // SI = 2;
            // MOS(SI);
            Write();
        }else if (IR[0] == 'L' && IR[1] == 'R')
        {
            Load_register();
        }else if (IR[0] == 'S' && IR[1] == 'R')
        {
        // SI = 4;
        // MOS(SI);
        Store_register();
        }else if (IR[0] == 'C' && IR[1] == 'R')
        {
        // SI = 5;
        // MOS(SI);
        Compare_register();
        }else if (IR[0] == 'B' && IR[1] == 'T')
        {
        // SI = 6;
        // MOS(SI);
        Branch_on_true();
       
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS(SI);
            return;
        }
        int address = (IR[2] - '0') * 10 + (IR[3] - '0'); 
        // if (returnAddress != -1 && IC != returnAddress && IC != address) {
        //     Halt(); 
        // }
    }
}


void OS::StartExe()
{
    IC = 0;
    Start_User_Pg();
}

void OS::LOAD() 
{
    int memPtr = 0;  // Persistent memory index
    bool loadMode = false;  // Only true between $AMJ and $DTA

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
            int k = 0;

            while (buffer[k] != '\0' && memPtr < 100 && k<40)
            {
                for (int j = 0; j < 4 && k<40; j++)
                {
                    if (buffer[k] == '\0'||k==40)
                        break;

                    M[memPtr][j] = buffer[k];

                    if (buffer[k] == 'H')  // Stop after halt
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
    for (int i = 0; i < 100; i++)
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