#include <iostream>
using namespace std;

class myString{
    friend ostream& operator << (ostream& s, myString& A);

    protected:
        char* strArray;
        int _size;

        void strCpy(char* A, char* B, int n);
    
    public:
        myString();
        myString(char* inputString);
        myString(myString& B);
        int Size();

        bool operator == (myString& s);
        bool operator > (myString& s);
        bool operator < (myString& s);
        myString& operator = (myString& s);
        myString& operator = (char* s);
};


////////////////////////////

class setOfURLs{

    private:
        int binarySearchAndInsert(myString& s);
    
    protected:
        myString* _URLs;
        int* _frequencies;
        int _size;
    
    public:
        setOfURLs();
        void addURL(myString& s);
        void sortFreq(); // sort URLs based on their frequency
        void sortURLs(); // sort _URLs alphabetically
        setOfURLs* removeURLs(myString* URLsToFilterOut); // remove URLsToFilterOut from URLs
        ~setOfURLs();
};

////////////////////////////////////

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
        void addNeighbor(URLLinks& link); // may be pointer instead?

};

/////////////////////////////////////

int main(){

    return 0;
}