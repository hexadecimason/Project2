#include <iostream>
using namespace std;

// used to empty a given string A of length n
void emptyString (char* A, int n) {
	for (int i=0; i < n; i++) {
		A[i] = '\0';
	}
}

// copy a string A to B of length n
void stringCopy (char* A, int n, char* B) {
	for (int i=0; i < n; i++) {
		B[i] = A[i];
	}
}

// return the length of a given string A
int stringLength (char* A) {
	int size = 0;
	while (A[size] != '\0') size++;
	return size; //size is the last index position
}







class myStringException : public exception{};

class myString{
    friend ostream& operator << (ostream& s, myString& A);

    protected:
        char* strArray;
        int size;
        void strCpy(char* A, char* B, int n);
    public:
        myString();
        myString(char* inputString);
        myString(myString& s);
        int Size();
        char* getWord();
        bool operator == (myString& s);
        bool operator > (myString& s);
        bool operator < (myString& s);
        myString& operator = (myString& s);
        myString& operator = (char* s);
};

// outputs a given string A
ostream& operator << (ostream& s, myString& A) {
    for(int i = 0; i < A.size; i ++){
        s << A.strArray[i];
    }
	return s;
}

void myString::strCpy(char* A, char* B, int n){
    // this assumes A and B are both of length n.
    for(int i = 0; i < n; i ++){
        A[i] = B[i];
    }
}

// default constructor
myString::myString () {
	size = 0;
	strArray = new char[1];
	strArray[0] = '\0';
}

myString::myString(char* inputString){
    int i = 0;
	while (inputString[i] != '\0')
		i++;
	size = i;
	strArray = new char[size];
	emptyString (strArray, size+1); // uses size+1 to maintain presence of null character at end of string: because size itself is not incremented, indexing through the array won't overwrite this "+1" character.
	for (int j=0; j < size; j++)
		strArray[j] = inputString[j];
}

myString::myString (myString& B) {
	delete [] strArray;
	strArray = NULL;
	size = B.size;
	strArray = new char[size];
	emptyString(strArray, size+1);
	stringCopy (B.strArray, size, strArray);
}

char* myString::getWord(){
    return strArray;
}

int myString::Size(){
    return size;
}

bool myString::operator == (myString& s){
    // checking equal sizes is an easy first way to check.
    if(this->size != s.Size()){
        return false;
    }
    else{
        for(int i = 0; i < this->size; i++){
            if(this->strArray[i] != s.strArray[i]){
                return false;
            }
        }
        return true;
    }
}

bool myString::operator > (myString& s){
    if(*this == s){
        return false;
    }
    else if(this-> size >= s.size){
        for(int i = 0; i < s.size; i++){
            if(this->strArray[i] > s.strArray[i])
                return true;
            else if(this->strArray[i] < s.strArray[i])
                return false;
        }

        // from here, one word is longer and both words are equal up until that point. Let the longer word be greater than by definition.
        return true;
    }
    else{
        // s has larger size
        for(int i = 0; i < this->size; i++){
            if(this->strArray[i] > s.strArray[i])
                return true;
            else if(this->strArray[i] < s.strArray[i])
                return false;
        }

        // both words equal up to the length of *this, but s is longer.
        return false;


    }
}

bool myString::operator < (myString& s){
    if(!(*this > s) && !(*this == s))
        return true;
    return false;
}

myString& myString::operator = (myString& s){
    strCpy(this->strArray, s.strArray, s.size);
    return *this;
}

myString& myString::operator = (char* s){
    strCpy(this->strArray, s, stringLength(s));
    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////


bool isValidURL(char* s, int size){
    bool tag = true;
    string tag1 = "http://";
    string tag2 = "https://";

    if(size < 8) return false;

    for(int i = 0; i < 8; i++){
        if(s[i] != tag1[i] && s[i] != tag2[i])
            tag = false;
    }

    return tag;
}

// get one URL from redirected input and return it as a string
char* getNextURL () {
	char* str = new char[50]; //assumes a max URL size of 50
	emptyString (str, 50);

	char c;
	int i = 0;
	//read until the next white space or line-break 
	while (!cin.eof()) {
		cin.get(c);
		if (!cin.eof ()) {
			if ((c != '\n') && (c != ' ')) {
				if ( ((c >= 'a') && (c <= 'z')) ||
					 ((c >= 'A') && (c <= 'Z')) ||
					 ((c >= '0') && (c <= '9')) ||
					 (c >= ':') || (c >= '/') || (c >= '.') || (c >= '_'))
					str[i++] = c;
			}
			else if ((c == '\n') && (i > 0)){
                if(isValidURL(str, i))
				    return str;
                else return getNextURL();
            }
			else if ((c == ' ') && (i > 0)){
                if(isValidURL(str, i))
				    return str;
                else return getNextURL();
            }
		}
	}
	return NULL;
}





class setOfURLsException : public exception{};

class setOfURLs{

    private:
        int binarySearchAndInsert(myString& s);
        void incCapacity();
        void decCapacity();
    
    protected:
        myString* _URLs;
        int* _frequencies;
        int _size;
    
    public:
        setOfURLs();
        setOfURLs(int numWords);

        myString* get_Words(); // pointer to an array of myString objects
        int* get_Freq();
        int get_size();

        void setSize(int i);

        void addURL(myString& s); // insert new myString into array, maintaining order
        void sortFreq(); // sort URLs based on their frequency
        void sortURLs(); // sort _URLs alphabetically
        void display();
        setOfURLs* removeURLs(myString* URLsToFilterOut, int numURLsToFilterOut); // remove URLsToFilterOut from URLs
        ~setOfURLs();
};

// protected members initialized to 0
setOfURLs::setOfURLs()
{
	_size = 0;
	_URLs = new myString[0];
	_frequencies = new int[0];
}

// non default constructor - initializes with a known number of words
setOfURLs::setOfURLs (int numOfWords)
{
	_size = numOfWords;
	_URLs = new myString[numOfWords];
	_frequencies = new int[numOfWords];
}

// Destructor
setOfURLs::~setOfURLs(){
    delete [] _URLs;
    delete [] _frequencies;
    _size = 0;
}

myString* setOfURLs::get_Words()
{
	return _URLs;
}

int* setOfURLs::get_Freq()
{
	return _frequencies;
}

int setOfURLs::get_size()
{
	return _size;
}

void setOfURLs::setSize(int i)
{
	_size = i;
}

void setOfURLs::display()
{
    for(int i = 0; i < _size; i ++){
        cout << _URLs[i] << endl;
    }
}

// sort the _URLs and _frequencies based on frequencies
void setOfURLs::sortFreq()
{   
    myString temp_url;
    int temp_f;

    for(int i = 0; i < _size; i++){
        for(int j = 0; j < _size; j++){
            if(_frequencies[j] > _frequencies[i]){
                temp_url = _URLs[i];
                temp_f = _frequencies[i];

                _URLs[i] = _URLs[j];
                _frequencies[i] = _frequencies[j];

                _URLs[j] = temp_url;
                _frequencies[j] = temp_f;
            }
        }
    }
}

// sort the _URLs and _frequencies, alphabetically
void setOfURLs::sortURLs()
{
    myString temp_url;
    int temp_f;

    for(int i = 0; i < _size; i++){
        for(int j = 0; j < _size; j++){
            if(_URLs[j] > _URLs[i]){
                temp_url = _URLs[i];
                temp_f = _frequencies[i];

                _URLs[i] = _URLs[j];
                _frequencies[i] = _frequencies[j];

                _URLs[j] = temp_url;
                _frequencies[j] = temp_f;
            }
        }
    }
}

setOfURLs* setOfURLs::removeURLs(myString* URLsToFilterOut, int numURLsToFilterOut)
{
    for(int i = 0; i < numURLsToFilterOut; i ++){
        for(int u = 0; u < _size; u++){
            if(URLsToFilterOut[i] == _URLs[u]){
                for(int r = u; r < _size+1; r++){
                    _URLs[r] = _URLs[r+1];
                    _frequencies[r] = _frequencies[r+1];
                }
                _URLs[_size-1].~myString(); // call destructor on final entry in memory.
                _frequencies[_size-1] = 0;
                _size--;
            }
        }
    }
    return this;
}

// to search for a given word in _URLs - returns 0 if not found, 1 if found
int setOfURLs::binarySearchAndInsert (myString& s)
{
    // for a set with nothing in it, we don't need to search.
    if(_size == 0){
        incCapacity();
        _URLs[0] = s;
        _frequencies[0] = 1;
        return 0;
    }

    // FIRST: find a potential existing value.
    int mid;
    int L = 0;
    int R = _size-1;

    while(L <= R){
        mid = (L+R)/2;

        if(_URLs[mid] < s)
            L = mid + 1;
        else if(_URLs[mid] > s)
            R = mid - 1;
        else if(_URLs[mid] == s){
            // found an existing URL, increment its frequency and return 1.
            _frequencies[mid] ++;
            cout << "duplicate found" << endl; ////////////////////////////////////////////////////////////////////
            return 1;
        }
    }

    incCapacity();
    for(int i = _size - 1; i > mid; i --){
        _frequencies[i] = _frequencies[i-1];
        _URLs[i] = _URLs[i-1];
    }

    _frequencies[mid] = 1; // just added, has a frequency of 1 for now.
    _URLs[mid] = s;
	
    return 0;
}

// method to add words to the setOfURLs object
void setOfURLs::addURL(myString & s)
{
    sortURLs(); // ensures we have correct sorting before attempting binary search/insert.
	binarySearchAndInsert(s);
}

void setOfURLs::incCapacity(){

    myString* tempURL = new myString[_size+1];
    int* tempF = new int[_size+1];

    for(int i = 0; i < _size; i ++){
        tempURL[i] = _URLs[i];
        tempF[i] = _frequencies[i];
    }
    
    delete [] _URLs;
    delete [] _frequencies;

    // reassign pointers to the newly allocated memory.
    _URLs = tempURL;
    _frequencies = tempF;

    _size ++;
}

void setOfURLs::decCapacity(){



}






/////////////////////////////////////////////////////////////////////////////////////////////////////////



/*
class URLLinks{

    friend ostream& operator << (ostream& s, URLLinks A);

    protected:
        myString URL;
        int numLinks;
        URLLinks** hyperLinks;

    public:
        URLLinks();
	    URLLinks(myString& x, int n);
	    ~URLLinks();
	    int getNumLinks();
	    URLLinks* getHyperLink(int i);
	    myString& getURL();
	    void addSite(myString& t);
	    void addNeighbor(URLLinks& link);
	    void setNeighbors(int nei);
};

ostream& operator << (ostream& s, URLLinks& A)
{
	//TODO
}

URLLinks::URLLinks()
{
	//TODO
}

URLLinks::URLLinks(myString& x, int n)
{
	//TODO
}

myString& URLLinks::getURL()
{
	//TODO
}

int URLLinks::getNumLinks()
{
	//TODO
}

URLLinks* URLLinks::getHyperLink(int i)
{
	//TODO
}

URLLinks::~URLLinks()
{
	//TODO
}

void URLLinks::addSite(myString& t)
{
	//TODO
}

void URLLinks::setNeighbors(int nei)
{
	//TODO
}

void URLLinks::addNeighbor(URLLinks& link)
{
	//TODO
}


*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////




int main(){

    int numURLsToFilterOut;
	char* url;
	myString* urlString;
	int numPages;
	int pageNo;
	int numNeighbors;
	int neighbor;

    cin >> numURLsToFilterOut;
	cout << "Number of Stop words: " << numURLsToFilterOut << endl;
	myString* URLsToFilterOutList = new myString[numURLsToFilterOut];

    //read the stop words
	for (int i=0; i < numURLsToFilterOut; i++)
	{
		url = getNextURL ();
		URLsToFilterOutList[i] = url; //calls the overloaded = operator on myString class
	}

    //Now read a text and put them in the setOfURLs instance.
	setOfURLs* mySetOfURLs = new setOfURLs ();

    url = getNextURL (); 

	while (url != NULL)
	{
		urlString = new myString (url); //create a myString object with the URL
		(*mySetOfURLs).addURL(*urlString); //add URL to mySetOfURLs
		url = getNextURL ();

        //cout << "Next URL: " << url << endl; ///////////////////////////////////////////////////////
	}

    // this should display the URL and frequency;
	// note that becuase you are using binary search and insert the URLs will
	// be sorted alphabetically
	cout << endl;
	cout << "Input display:" << endl;
	(*mySetOfURLs).display ();

	(*mySetOfURLs).sortFreq ();
	cout << endl;
	cout << "mySetOfURLs - Sorted based on frequency:" << endl;
	(*mySetOfURLs).display ();

	(*mySetOfURLs).sortURLs();
	cout << endl;
	cout << "mySetOfURLs - Sorted alphabetically:" << endl;
	(*mySetOfURLs).display ();

	setOfURLs* newSetOfURLs = (*mySetOfURLs).removeURLs(URLsToFilterOutList, numURLsToFilterOut); // new parameter added here
	cout << endl;
	cout << "newSetOfURLs - Sorted alphabetically:" << endl;
	(*newSetOfURLs).display();

	(*newSetOfURLs).sortFreq ();
	cout << endl;
	cout << "newSetOfURLs - Sorted based on frequency:" << endl;
	(*newSetOfURLs).display ();


    return 0;
}