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



// #####################################################################################################################

/*

*/

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
    s << A.strArray;
	return s;
}

// default constructor
myString::myString () {
	size = 0;
	strArray = new char[1];
	strArray[0] = '\0';
}

// constructor that uses a character array
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

// copy constructor creates a new instance from an existing object
myString::myString (myString& B) {
	size = B.size;
	strArray = new char[size];
	emptyString(strArray, size+1);
	stringCopy (B.strArray, size, strArray);
}

// returns the array containing the characters of the myString() instance.
char* myString::getWord(){
    return strArray;
}

// returns the "length" of the string (size of array)
int myString::Size(){
    return size;
}

// overloaded equality operator
bool myString::operator == (myString& s){
    // checking equal sizes to avoid unnecessary looping.
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

// overloaded greater than - uses relative ASCII values
bool myString::operator > (myString& s){
    // check for equality first
    if(*this == s){
        return false;
    }
    // *this has larger size
    else if(this-> size >= s.size){
        for(int i = 0; i < s.size; i++){
            if(this->strArray[i] > s.strArray[i])
                return true;
            else if(this->strArray[i] < s.strArray[i])
                return false;
        }

        // both words equal up to length of 's,' but *this is longer and therefore "greater than," by our definition
        return true;
    }
    // s has larger size
    else{
        for(int i = 0; i < this->size; i++){
            if(this->strArray[i] > s.strArray[i])
                return true;
            else if(this->strArray[i] < s.strArray[i])
                return false;
        }

        // both words equal up to length of *this, but 's' is longer and therefore "greater than," by our definition
        return false;


    }
}

// overloaded less than - uses relative ASCII values
bool myString::operator < (myString& s){
    // if not greater than or equal, must be less than.
    if(!(*this > s) && !(*this == s))
        return true;
    return false;
}

// overloaded assignment operator to perform deep copy
myString& myString::operator = (myString& s){
    delete [] strArray;
    size = stringLength(s.strArray);
    strArray = new char[size];
    emptyString(strArray, size+1);
    stringCopy(s.strArray, size, strArray);
    return *this;
}

// assignment operator for a character array resizes existing instance
myString& myString::operator = (char* s){
    delete [] strArray;
    size = stringLength(s);
    strArray = new char[size];
    emptyString(strArray, size+1);
    stringCopy(s, size, strArray);
    return *this;
}



// #####################################################################################################################


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
        if(c == '$'){ // this is how we recognize the '$$$$'
            while(c == '$'){
                cin.get(c);
            }
            cin.get(c); // read extra whitespace
            return NULL;
        }
		if (!cin.eof ()) {
			if ((c != '\n') && (c != ' ')) {
				if ( ((c >= 'a') && (c <= 'z')) ||
					 ((c >= 'A') && (c <= 'Z')) ||
					 ((c >= '0') && (c <= '9')) ||
					 (c >= ':') || (c >= '/') || (c >= '.') || (c >= '_'))
					str[i++] = c;
			}
			else if ((c == '\n' || c == ' ') && (i > 0)){
                if(isValidURL(str, i))
				    return str;
                else return getNextURL();
            }/*
			else if ((c == ' ') && (i > 0)){
                if(isValidURL(str, i))
				    return str;
                else return getNextURL();
            }*/
		}
	}
	return NULL;
}






class setOfURLsException : public exception{};

class setOfURLs{

    private:
        int binarySearchAndInsert(myString& s);
        void deleteURL(int i);
        void incCapacity();
        void decCapacity();
        void urlSwap(int i, int j);
    
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
    cout << "mySetOfURLs: " << _size << endl;
    for(int i = 0; i < _size; i ++){
        cout << _URLs[i] << ": " << _frequencies[i] << endl;
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
                urlSwap(i, j);
            }

            if(_frequencies[i] == _frequencies[j] && _URLs[j] > _URLs[i]){
                urlSwap(i, j);
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
                urlSwap(i, j);
            }
        }
    }

}

setOfURLs* setOfURLs::removeURLs(myString* URLsToFilterOut, int numURLsToFilterOut)
{
    for(int i = 0; i < numURLsToFilterOut; i ++){
        for(int u = 0; u < _size; u++){
            if(URLsToFilterOut[i] == _URLs[u]){
                deleteURL(u);
            }
        }
    }
    return this;
}

void setOfURLs::deleteURL(int i){

    for(int k = i; i < _size - 1; i ++){
        _URLs[i] = _URLs[i+1];
        _frequencies[i] = _frequencies[i+1];
    }

    decCapacity();
    
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

    int incSize = _size + 1;
    myString* tempURL = new myString[incSize];
    int* tempF = new int[incSize];

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

    _size --;
    myString* tempURL = new myString[_size];
    int* tempF = new int[_size];

    for(int i = 0; i < _size; i ++){
        tempURL[i] = _URLs[i];
        tempF[i] = _frequencies[i];
    }

    delete [] _URLs;
    delete [] _frequencies;

    _URLs = tempURL;
    _frequencies = tempF;
}

void setOfURLs::urlSwap(int i, int j){
    myString temp_url;
    int temp_f;

    temp_url = _URLs[i];
    temp_f = _frequencies[i];

    _URLs[i] = _URLs[j];
    _frequencies[i] = _frequencies[j];

    _URLs[j] = temp_url;
    _frequencies[j] = temp_f;
}




// #####################################################################################################################


class URLLinksException : public exception {};

class URLLinks{

    friend ostream& operator << (ostream& s, URLLinks& A);

    private:
        void resizeNumLinks(int newSize);
        int filled;
    protected:
        myString URL;
        int numLinks;
        URLLinks** hyperLinks;
    public:
        URLLinks();
        URLLinks(int n);
	    URLLinks(myString& x, int n);
	    ~URLLinks();
	    int getNumLinks();
	    URLLinks* getHyperLink(int i);
	    myString& getURL();
	    void addSite(myString& t);
	    void addNeighbor(URLLinks& link);
	    void setNeighbors(int nei);
        URLLinks& operator = (URLLinks& u);
        URLLinks& operator [] (int i);
};

ostream& operator << (ostream& s, URLLinks& A)
{

    s << A.getURL() << ":" << endl;

    /*
	s << A.getURL() << ": " << A.getNumLinks() << endl;

    for(int i = 0; i < A.getNumLinks(); i ++){
        for(int k = 0; k < A.getHyperLink(k)->getNumLinks(); k ++){
            s << "** " << A.getHyperLink(k)->getURL() << endl;
        }
    }*/
    
    return s;
}

URLLinks::URLLinks()
{
	numLinks = 0;
    filled = 0;
    URL = *(new myString());
    hyperLinks = new URLLinks*[0];
}

URLLinks::URLLinks(myString& x, int n)
{
	URL = x;
    numLinks = n;
    filled = 0;
    hyperLinks = new URLLinks*[n];
}

myString& URLLinks::getURL()
{
	return URL;
}

int URLLinks::getNumLinks()
{
	return numLinks;
}

URLLinks* URLLinks::getHyperLink(int i)
{
	return hyperLinks[i];
}

URLLinks::~URLLinks()
{   
    numLinks = 0;
    delete [] hyperLinks;
}

void URLLinks::addSite(myString & t)
{
    resizeNumLinks(numLinks + 1);
    hyperLinks[numLinks - 1] = new URLLinks(t, 0);
}

void URLLinks::setNeighbors(int nei)
{
    resizeNumLinks(nei);
}

void URLLinks::resizeNumLinks(int newSize){

    // make new expanded array
	URLLinks ** hyperTemp = new URLLinks*[newSize];

    // if new size is smaller, it cuts off values.
    if(newSize >= numLinks){
        for(int i = 0; i < numLinks - 1; i ++){
            hyperTemp[i] = hyperLinks[i];
        }
    }
    else{
        for(int i = 0; i < newSize - 1; i ++){
            hyperTemp[i] = hyperLinks[i];
        }
    }
    
    numLinks = newSize;

    delete [] hyperLinks;
    hyperLinks = hyperTemp;
}

void URLLinks::addNeighbor(URLLinks& link)
{   
    if(filled >= numLinks){
        int diff = filled - numLinks;
        resizeNumLinks(numLinks + diff + 1);
    }

    hyperLinks[filled] = &link;
    filled ++;
}


URLLinks& URLLinks::operator = (URLLinks& u){
    
    URL = u.getURL();
    numLinks = u.getNumLinks();
    resizeNumLinks(numLinks);

    for(int i = 0; i < numLinks; i ++){
        hyperLinks[i] = u.getHyperLink(i);
    }

    return *this;
}

URLLinks& URLLinks::operator [] (int i){

    return *hyperLinks[i];

}







// #####################################################################################################################




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
	}

    // this should display the URL and frequency;
	// note that becuase you are using binary search and insert the URLs will
	// be sorted alphabetically
    {
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
    }
	

    cin >> numPages;
	cout << "Number of websites: " << numPages << endl;


    url = getNextURL (); // prime for new URL;
	URLLinks* myLinkStructure = new URLLinks[numPages];

    // assign URLs to out URLLinks structure.
	for (int i = 0; i < numPages-1; i++)
	{
		// read all URL and store them in the myLinkStructure array of URLLink objects
        urlString = new myString (url);
        myLinkStructure[i] = *(new URLLinks(*urlString, 0));
//cout << "hyperlinked to myLinkStructure: " << myLinkStructure[i] << endl;
		url = getNextURL ();
	}




    // store the neighbours/hyperlinks of just-added URLs
	for (int i = 0; i < numPages; i++)
	{
		cin >> pageNo >> numNeighbors;
		myLinkStructure[i].setNeighbors(numNeighbors);
		for (int j = 0; j < numNeighbors; j++)
		{
			cin >> neighbor;
			myLinkStructure[pageNo].addNeighbor((myLinkStructure[neighbor]));
		}
	}





	cout << "~~~Webpages and the links they contain:" << endl;
	// display all URLLink objects using the overloaded << operator (see sample output file)

    for(int i = 0; i < numPages; i ++){
        cout << myLinkStructure[i];

        for(int k = 0; k < myLinkStructure[i].getNumLinks(); k ++){
            cout <<  "** " << myLinkStructure[i].getHyperLink(k)->getURL() << endl;
        }

        cout << endl;
            
    }





	cout << "~~~Webpages and the number and URL of pages that contain links to them:" << endl;
	// display all the incoming nodes here (see sample output file)

    int count;
    myString * testSet = new myString[numPages];

    for(int i = 0; i < numPages; i ++){

        count = 0;
        myString rootURL = myLinkStructure[i].getURL();

        for(int j = 0; j < numPages; j ++){

            for(int k = 0; k < myLinkStructure[j].getNumLinks(); k ++){
                
                myString checkURL = myLinkStructure[j].getHyperLink(k)->getURL();

                if(rootURL == checkURL && i != j){

                    testSet[count] = myLinkStructure[j].getURL();
                    count ++;
                }
            }
        }

        cout << rootURL << ": " << count << endl;

        for(int p = 0; p < count; p ++){
            cout << "** " << testSet[p] << endl;
        }

        cout << endl;

    }

	// TODO : destructors

    delete [] testSet;
	delete [] URLsToFilterOutList;
	delete mySetOfURLs;
	delete [] myLinkStructure;
    delete [] urlString;
    delete [] url;

	return 0;
}