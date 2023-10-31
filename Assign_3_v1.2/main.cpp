#include "wordfrequency.h"
 
 /** 
  * Assignment 3 for COSE213 Data Structures
  *
  * Won-Ki Jeong (wkjeong@korea.ac.kr)
  *
  * 2022. 5. 1
  *
  */
 
using namespace std;

int main()
{
	typedef MapElem<std::string,int> ElemType;
	typedef HashMap<WordFreqElem> MapType;	

	// Map Test
	MapType myMap;
	
	myMap.insert("James", 35);
	myMap.insert("Tom", 12);
	myMap.insert("Kevin", 27);
	myMap.insert("Jessica", 43);
	myMap.print();
	
	
	// Word Frequency Test
	WordFrequency freqChecker;
	
	freqChecker.ReadText("steve_jobs.txt");
	
	cout << "Print all results" << endl;
	freqChecker.PrintAllFrequency();
	
	
	return 0;
}
