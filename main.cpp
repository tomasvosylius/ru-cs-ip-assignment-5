#include <iostream>
#include <fstream>          // for file I/O
#include <limits>
#include <cassert>          // for assertion checking

// Student name: Tomas Vosylius / 1042957

using namespace std;

enum Action {Encrypt, Decrypt} ;

int seed = 0 ;
void initialise_pseudo_random (int r)
{
    //  pre-condition:
    assert (r > 0 && r <= 65536) ;
    /**  post-condition:
        seed has value r.
    */
    seed = r ;
}

int next_pseudo_random_number ()
{
    //  pre-condition:
    assert (seed > 0 && seed <= 65536) ;
    /** post-condition:
        result value > 0 and result value <= 65536 and result value != seed at entry of function
    */
    const int seed75 = seed * 75 ;
    int next = (seed75 & 65535) - (seed75 >> 16) ;
    if (next < 0)
        next += 65537 ;
    seed = next ;
    return next ;
}

char rotate_char (char a, int r, Action action)
{
    //  Pre-condition:
    assert(true);
    /** Post-condition:
        returns a encrypted/decrypted value depending on Action and input (char a, integer r)
    */
    char result = a;
    if(a >= 32 && a < 128)
    {
        if(action == Encrypt)
        {
            result = (a-32 + (r % (128-32))) % (128-32) + 32;
        }
        else if(action == Decrypt)
        {
            result = (a-32 - (r % (128-32)) + (128-32)) % (128-32) + 32;
        }
    }
    return result;
}

void test_rotate_char()
{
    //  Pre-condition:
    assert(true);
    /** Post-condition:
        Function simply checks ASCII characters rotation using rotate_char function.
    */
    int r = -1;
    while(r <= 0)
    {
        cout << "Enter positive value for R:" << endl;
        cin >> r;

        if(r > 0)
        {
            char encrypted, decrypted;
            cout << "ID\tCH\tEN\tDE" << endl;
            for(int a = 32; a < 128; a++) // Starting only from 32, because 0-32 remains the same
            {
                encrypted = rotate_char(static_cast<char>(a), r, Encrypt);
                decrypted = rotate_char(encrypted, r, Decrypt);

                cout << a << "\t" << static_cast<char>(a) << "\t" << encrypted << "\t" << decrypted << endl;
            }
        }
        r = -1;
        // Ignoring character or string inputs, otherwise it'll give us infinite loop
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

bool open_input_and_output_file (ifstream& infile, ofstream& outfile)
{
    //  Pre-condition:
    assert(true);
    /** Post-condition:
        Function allows user to enter input&output file names.
        Then will open files and return
            true if files were opened successfully
            false if files weren't opened
        Function will pass streams by reference
    */

    string input,
           output;

    // User input
    cout << "Enter input file name (may not contain layout symbols):" << endl;
    cin >> input;

    cout << "Enter output file name (may not contain layout symbols):" << endl;
    cin >> output;

    // Checking if names are same
    if(output == input)
    {
        cout << "Error! Files can not have same name." << endl;
        return false;
    }

    // Opening the files
    infile.open(input.c_str());
    if(infile.fail())
    {
        cout << "Specified input file can not be opened!" << endl;
        return false;
    }
    outfile.open(output.c_str());
    if(outfile.fail())
    {
        cout << "Specified output file can not be opened!" << endl;
        return false;
    }
    return true;
}

Action get_user_action ()
{
    // Pre-condition:
    assert(true);
    /** Post-condition:
        result is the Action that the user has indicated that the program should encrypt / decrypt
    */
    cout << "Do you want to encrypt the file? (y/n): " ;
    string answer ;
    cin  >> answer ;
    if (answer == "y")
        return Encrypt;
    else
        return Decrypt;
}

int initial_encryption_value ()
{
    // Pre-condition:
    assert (true) ;
    /** Post-condition:
        result is a value between 0 and 65355 (both inclusive)
    */
    int initial_value = -1 ;
    while (initial_value < 0 || initial_value > 65535)
    {
        cout << "Please enter the initial coding value (greater or equal to 0 and less than 65536)" << endl ;
        cin  >> initial_value ;
    }
    return initial_value ;
}

void use_OTP (ifstream& infile, ofstream& outfile, Action action, int initial_value)
{
    //  Pre-condition:
    assert(infile && outfile && initial_value > 0 && initial_value <= 65535);
    /** Post-condition:
        Function reads every character on infile and rotates it (depending on Action).
        Then puts encrypted/decrypted output to outfile.
        initial_value is initial R value
    */
    char
        read,
        changed;
    int
        r = initial_value;

    initialise_pseudo_random(r);

    infile.get(read);
    while(!infile.fail())
    {
        // Generate next seed and rotate character
        r       = next_pseudo_random_number();
        changed = rotate_char(read, r, action);

        // Print out rotated char to file
        outfile.put(changed);

        // Read next character
        infile.get(read);
    }
}

int main()
{
    // test_rotate_char();

    const Action ACTION = get_user_action() ;
    ifstream input_file  ;
    ofstream output_file ;
    if (!open_input_and_output_file (input_file,output_file))
    {
        cout << "Program aborted." << endl;
        return -1 ;
    }
    const int INITIAL_VALUE = initial_encryption_value () ;
    use_OTP (input_file,output_file,ACTION,INITIAL_VALUE);
    input_file.clear () ;
    output_file.clear () ;
    input_file.close () ;
    output_file.close () ;

    if (!input_file || !output_file)
    {
        cout << "Not all files were closed succesfully. The output might be incorrect." << endl;
        return -1 ;
    }
    return 0 ;
}
