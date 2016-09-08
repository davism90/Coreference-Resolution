/* 
 * File:   main.cpp
 * Author: Mariah Davis, A45311948
 *
 * Created on January 28, 2015, 2:47 PM
 */

#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

/*
 * 
 */
///////////////////////////The viterbi algorithm.///////////////////////////////

int viterbi(string trainingfile, string testingfile){
    //function reads in two files, grabs dictionary from one, calculates
    //likelihood of bigram for testing file based on training file.
    
    string trainingdata;
    string testingdata;
    string viterbiword;
    int i;
    int f;
    int count;
    float viterbimax;
    float viterbimax2;
    ifstream training;
    ifstream testing;
    vector<string> trainingwords;
    vector<string>testingwords;
    multimap<string, string>trainingbigrams;
    map<string, string>testingbigrams;
    multimap<string, string>::iterator h;
    map<string, string>::iterator g;
    
  //Create the bigrams and put them in a multimap for the dictionary.
    training.open(trainingfile);
    if (training.is_open()){
       
        while (training>>trainingdata){
            trainingwords.push_back(trainingdata);
            }

        for (i=1; i<trainingwords.size()-1; i++){
            trainingbigrams.insert(pair<string, string>((trainingwords[i]+" "+trainingwords[i+2]),
                    (trainingwords[i-1]+" "+trainingwords[i+1]))); 
            i+=1;
        }
    }
    testing.open(testingfile);
    if (testing.is_open()){
       
        while (testing>>testingdata){
            testingwords.push_back(testingdata);
        }
        for (f=1; f<testingwords.size()-1; f++){
            testingbigrams.insert(pair<string, string>(testingwords[f-1], testingwords[f])); 
            f+=1;
        }
        
        for (h=trainingbigrams.begin();h!=trainingbigrams.end(); h++){
            for (g=testingbigrams.begin();g!=testingbigrams.end(); g++){
                if ((g->first+" "+g->second)==h->second){
                        viterbimax=trainingbigrams.count(h->first);
                        if (viterbimax>viterbimax2 && (h->second) != viterbiword 
                                && (h->second != "UNKA")){
                                viterbimax2=viterbimax;
                                viterbiword=(h->first);
                                
                                cout<<"The tags for bigram '"<<h->second<<"' are '"
                                        <<viterbiword<<"'."<<endl;
                                }
                        else
                                viterbimax=viterbimax;
                                
                                }
                          }
            
                    }

                }
    return 0;
}
        

                                
  
                
        


////////////////////////////////////////////////////////////////////////////////
//From here on, the code calculates probabilities of bigrams and unigrams.//////
////////////////////////////////////////////////////////////////////////////////

string transitionpt3 (string filename, string word, string secondword, string tag, 
        string secondwordtag, float lambda){
    vector<string> words;
    int counttotal=0;
    int i;
    string data;
    string viterbiword;
    string viterbiresult;
    float viterbimax;
    float viterbimax2=0;
    ifstream testfile;
    multimap<string, string>bigrams;
    multimap<string, string>::iterator h;
    
    testfile.open(filename);
    if (testfile.is_open()){
/*create vector of the words for easy mapping*/        
        while (testfile>>data){
            words.push_back(data);
            }
        
    for (i=1; i<words.size()-2; i++){
            if (words[i-1]==word && words[i]==tag && words[i+1]!=secondword &&
                    words[i+2]!=secondwordtag){
                counttotal+=1;
                bigrams.insert(pair<string, string>((words[i-1]+" "+words[i+1]), 
                    (words[i]+" "+words[i+2]))); 

                }
           
            if (words[i-1]==word && words[i+1]==secondword && words[i]==tag
                    && words[i+2]==secondwordtag){
                counttotal+=1;       
                bigrams.insert(pair<string, string>((words[i-1]+" "+words[i+1]), 
                    (words[i]+" "+words[i+2]))); 
                }
            i++;
                
            }    
    }   
    for (h=bigrams.begin();h!=bigrams.end(); h++){
        //cout<<endl<<h->first<<" ||||||||||"<<h->second<<endl;
        viterbimax=bigrams.count(h->first);

        if (viterbimax>viterbimax2 && (h->first) != viterbiword && (h->
                first != "UNKA")){
                viterbimax2=viterbimax;
                viterbiword=(h->first);
                }
        else
            viterbimax=viterbimax;
        }
    
    if((counttotal-1)<=0){
        viterbimax2= viterbimax2/counttotal;
    }
    else 
        viterbimax2=(1-(viterbimax-lambda)/counttotal);
    
    cout<<"The most likely transition of "<<"'"<<word<<"'"<<
            " is "<<"'"<<viterbiword<<"'"<<" with a "<<(viterbimax2*100)<< "% chance"
            " of occuring."<<endl<<endl;
    return ("Calculation Complete");
        
}

float transitionpt2 (string filename, string word, string secondword, string tag, 
        string secondwordtag, float lambda){
    string data;
    int i;
    float wordsandtags;
    float transitionprobability;
    float count=0;
    float count2=0;
    ifstream testfile;
    vector<string> words;
    multimap<string, string>bigrams;
    multimap<string, string>::iterator j;
    

    testfile.open(filename);
    if (testfile.is_open()){
     
        while (testfile>>data){
            words.push_back(data);
            }

        for (i=1; i<words.size()-2; i++){
            if (words[i-1]==word && words[i]==tag && words[i+1]!=secondword &&
                    words[i+2]!=secondwordtag){
                count++;
                bigrams.insert(pair<string, string>((words[i-1]+" "+words[i+1]), 
                    (words[i]+" "+words[i+2]))); 
                }
          
            if (words[i-1]==word && words[i+1]==secondword && words[i]==tag
                    && words[i+2]==secondwordtag){
                count2++;
                count++;
                bigrams.insert(pair<string, string>((words[i-1]+" "+words[i+1]), 
                    (words[i]+" "+words[i+2]))); 
                }
            i++; 
            }     
        }
    /*Discounting method smoothing*/
    if((count2-1)<=0){
        return count2/count;
    }
    else 
        return(1-(count2-lambda)/count);

}

float transitionpt1 (multimap<string, string>givenmap, string word, string tag){
    float numwordsindoc=(givenmap.size());
    float wordsandtags;
    float emissionprobability;
    multimap<string, string>::iterator j;
    
    for (j=givenmap.begin();j!=givenmap.end(); j++ ){
        if (j->second == tag && j->first == word){
            wordsandtags+=1;
            //cout<<j->first<<"|"<<j->second<<endl;
        }
    }
    emissionprobability=(wordsandtags/numwordsindoc);
    return emissionprobability;
}

multimap <string, string> bigrammap(string filename){
    string data;
    int i;
    ifstream testfile;
    vector<string> words;
    multimap<string, string>bigrams;
    multimap<string, string>::iterator j;
       
    testfile.open(filename);
    if (testfile.is_open()){       
        while (testfile>>data){
            words.push_back(data);
            }

        for (i=1; i<words.size()-1; i++){
            bigrams.insert(pair<string, string>((words[i-1]+" "+words[i+1]), 
                    (words[i]+" "+words[i+2]))); 
            i+=1;
        //cout<<words[i]<<endl;
                }
        }
    /*for (j=bigrams.begin();j!=bigrams.end(); j++ ){
        cout<<j->first<<"|"<<j->second<<endl;
    }*/
    return bigrams;
}

float emission (multimap<string, string>givenmap, string word, string tag){
    /*finds probability of given word and tag appearing in data, commented out 
     testing procedures*/
    float numwordsindoc=(givenmap.size());
    float wordandtag;
    float emissionprobability;
    multimap<string, string>::iterator j;
    
    for (j=givenmap.begin();j!=givenmap.end(); j++ ){
        if (j->second == tag && j->first == word){
            wordandtag+=1;
            //cout<<j->first<<"|"<<j->second<<endl;
        }
    }
    emissionprobability=(wordandtag/numwordsindoc);
    return emissionprobability;
}


multimap <string, string> unigrammap(string filename){
    string data;
    int i;
    ifstream testfile;
    vector<string> words;
    multimap<string, string>unigrams;
    multimap<string, string>::iterator j;
    
/*open the file*/    
    testfile.open(filename);
    if (testfile.is_open()){
/*create vector of the words for easy mapping*/        
        while (testfile>>data){
            words.push_back(data);
            }
/*loop to append to map, commented out testing procedures*/
        for (i=1; i<words.size(); i++){
            unigrams.insert(pair<string, string>(words[i-1], words[(i)])); 
            i+=1;
        //cout<<words[i]<<endl;
                }
        }
    /*for (j=unigrams.begin();j!=unigrams.end(); j++ ){
        cout<<j->first<<"|"<<j->second<<endl;
    }*/
    return unigrams;
}

int main(){
    string inputfile;
    string trainingfile;
    string testingfile;
    string word;
    string tag;
    string word2;
    string tag2;
    float lambda;
    cout<<"Please select a file to test."<<endl;
    cin>>inputfile;
    cout<<"Please select a word, for unigram calculations, case sensitive."<<endl;
    cin>> word;
    cout<<"Please select a tag, for unigram calculations, in all capitals."<<endl;
    cin>> tag;
    cout<<"Please select another word, for bigram calculations, case sensitive."<<endl;
    cin>> word2;
    cout<<"Please select another tag, for bigram calculations, in all capitals."<<endl;
    cin>> tag2;
    cout<<"Please select a lambda for smoothing."<<endl;
    cin>>lambda;
    cout<<endl<<"The emission probability of the given unigram and the given tag "
            "is "<<emission (unigrammap(inputfile), word, tag)<<"."
            <<endl;
    cout<<"The emission probability of the given bigram and the given tags "
            "is "<<transitionpt1(bigrammap(inputfile), word+" "+word2, 
            tag+" "+tag2)<<"."<<endl;
    cout<<"The transition probability of the given bigram, with respect to "
            "smoothing"<<endl<<"and given tags is "<<transitionpt2((inputfile),
            word, word2, tag, tag2, lambda)<<"."<<endl;
    cout<<transitionpt3((inputfile), word, word2, tag, tag2, lambda)
            <<"."<<endl;
   
    cout<<"Please select a file to use as training."<<endl;
    cin>>trainingfile;
    cout<<"Please select a file to test."<<endl;
    cin>>testingfile;
    viterbi(trainingfile, testingfile);
    return 0;
}



