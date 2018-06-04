#include<iostream>
#include<string>
#include<algorithm>
#include<ctype.h>
#include<sstream>
#include<vector>
#include<fstream>
#include<math.h>
#include<stdio.h>
#include<set>
#include<iterator>

using namespace std;

/*bool isInVocab(vector<string> vocabulary, string word)
{
	bool result = 0;
	for (int i = 0; i < vocabulary.size(); i++)
		if (vocabulary.at(i) == word)
			result = 1;
	
		
	return result;
}*/

long double probabilityOfRecordsWithClassAndSentence(int _class, vector<int> sentence, vector<int> prob_key1, vector<int> prob_key0, int end_of_training, int class_count, int v_size)
{
	long double probability = 0;
	long double records_with_both = 0;
	long double records_with_one = 0;
	//int class_count = 0;
	vector<int> temp = sentence;
	temp.at(temp.size() - 1) = _class;
	for (int i = 0; i < temp.size()-1; i++)
	{
		records_with_both = 0;
		records_with_one = 0;
    if(_class == 0){
      //cout << prob_key.at(i);
      if(temp.at(i) == 1){
        records_with_both = prob_key1.at(i);
      }
      else{
        records_with_both = prob_key0.at(i);
      }
      //cout << records_with_both << " ";
    }
    else{
      if(temp.at(i) == 1){
        records_with_both = prob_key1.at(i+v_size);
      }
      else{
        records_with_both = prob_key0.at(i+v_size);
      }
    }
    if(temp.at(i) == 1){
      records_with_one = prob_key1.at(i) + prob_key1.at(i+v_size);
    }
    else{
      records_with_one = prob_key0.at(i) + prob_key0.at(i+v_size);
    }
    //cout << records_with_one << " ";
   // cout << (records_with_both + 1) <<" "<< (records_with_one + 2) << " " <<((records_with_both + 1) / (records_with_one + 2)) << endl;
		probability += log(((records_with_both + 1) / (records_with_one + 2)));
	}
	probability += log((class_count / end_of_training + 1));
	
//	cout << probability << endl;
	return probability;
}

void fillProbKey1(vector<int> &prob_key, vector<vector<int> > training_data, int end_of_training, int v_size, int &num_class_0, int &num_class_1){
  for (int i = 0; i < end_of_training; i++){
    for (int j = 0; j < training_data.at(i).size() - 1; j++){
      if(training_data.at(i).at(j) == 1){
        if(training_data.at(i).at(training_data.size() - 1) == 0){
          prob_key.at(j) += 1;
          
        }
        else{
          prob_key.at(j+v_size) += 1;
        }
      }
    }
    if(training_data.at(i).at(v_size) == 0){
      num_class_0 += 1;
          
    }
    else{
      num_class_1 += 1;
    }
  }
}
void fillProbKey0(vector<int> &prob_key, vector<vector<int> > training_data, int end_of_training, int v_size){
  for (int i = 0; i < end_of_training; i++){
    for (int j = 0; j < training_data.at(i).size() - 1; j++){
      if(training_data.at(i).at(j) == 0){
        if(training_data.at(i).at(training_data.size() - 1) == 0){
          prob_key.at(j) += 1;
          
        }
        else{
          prob_key.at(j+v_size) += 1;
        }
      }
    }
  }
}

int main (int argc, char** argv)
{
	if (argc > 3 || argc < 3)
	{
		cout << "Incorrect number of arguments" << endl;
		return 1;
	}
	ifstream training_data, testing_data;
	
	training_data.open(argv[1]);
	testing_data.open(argv[2]);
	
	set<string> vocabulary;
	string line, result;
	string::size_type i;
	
	cout << "Please allow one minute for data collection" << endl;
	
	while(!training_data.eof())
	{
    result = "";
		getline(training_data, line);
		i = line.find("\t");
		if (i != string::npos)
			line.erase(i, line.length());
		remove_copy_if(line.begin(), line.end(), back_inserter(result), ptr_fun<int, int>(&ispunct));
		istringstream iss(result);
		do {
			string sub;
			iss >> sub;
			transform(sub.begin(), sub.end(), sub.begin(), ::tolower);
		//	if (vocabulary.count(sub) == 0)
				vocabulary.insert(sub); //insert on a set does this check for us
		} while (iss);
	}
	vocabulary.erase("0");
  vocabulary.erase("1");
	//cout << "Sorting vocabulary alphabetically." << endl;
	//sort(vocabulary.begin(), vocabulary.end());
	
	vector<vector<int> > features;
	vector<int> generated_class_labels;
	string line_no_punct;
	int class_label;
	int end_of_training = 0;
	int v_size = vocabulary.size();

	//cout << v_size << endl;
	
	cout << "Generating features for training_data" << endl;
	training_data.close();
	training_data.open(argv[1]);
	
	while (!training_data.eof())
	{
		vector<int> temp;
    line_no_punct = "";
    result = "";
   
		getline(training_data, line);
		remove_copy_if(line.begin(), line.end(), back_inserter(line_no_punct), ptr_fun<int, int>(&ispunct));
    if(line_no_punct[line_no_punct.size()-2] == '0'){ 
      class_label = 0;
    }
    else{
      class_label = 1;
    }
    line_no_punct[line_no_punct.size()-2] = ' ';
    result = line_no_punct;
		//sscanf(line_no_punct.c_str(), "%s \t %d", result.c_str(), &class_label);
  // cout <<"line: "<<line<< " line no punct: " << line_no_punct << " class label: " << class_label << endl;
  // cout << "result: " << result << endl;
   
		istringstream iss(result);
    
		temp.resize(v_size+1,0);
		do {
			string sub;
			iss >> sub;
			transform(sub.begin(), sub.end(), sub.begin(), ::tolower);
      if(vocabulary.count(sub) == 1){
			  int ind = distance(vocabulary.begin(), vocabulary.find(sub));
			  if(temp[ind] == 0){
				  temp[ind] = 1; 
			  }
      }
			/*for (int j = 0; j < v_size; j++)
			{
				if (sub == vocabulary.at(j))
					temp.push_back(1);
				else
					temp.push_back(0);
			}*/
			//temp.push_back(class_label);
		} while (iss);
   
		temp[v_size] = class_label;
    //cout << "class label: " << temp[v_size] << endl;
		end_of_training++;
		features.push_back(temp);
	}
  
   
   /*for(int i=0; i<features.at(0).size(); i++){
     cout << features.at(0).at(i) << " ";
   }
   cout << endl;*/
	
//	if (strcmp(argv[1], argv[2]) == 0) 
	//{
		cout << "Generating features for testing data" << endl;
	while (!testing_data.eof())
	{
		vector<int> temp;
    line_no_punct = "";
    result = "";
   
		getline(testing_data, line);
		remove_copy_if(line.begin(), line.end(), back_inserter(line_no_punct), ptr_fun<int, int>(&ispunct));
    if(line_no_punct[line_no_punct.size()-2] == '0'){
      class_label = 0;
      //cout << "class label 0" << endl;
    }
    else{
      //cout << "class label 1" << endl;
      class_label = 1;
    }
    line_no_punct[line_no_punct.size()-2] = ' ';
    result = line_no_punct;
		//sscanf(line_no_punct.c_str(), "%s \t %d", result.c_str(), &class_label);
  // cout <<"line: "<<line<< " line no punct: " << line_no_punct << " class label: " << class_label << endl;
  // cout << "result: " << result << endl;
   
		istringstream iss(result);
    
		temp.resize(v_size+1,0);
		do {
			string sub;
			iss >> sub;
			transform(sub.begin(), sub.end(), sub.begin(), ::tolower);
      if(vocabulary.count(sub) == 1){
			  int ind = distance(vocabulary.begin(), vocabulary.find(sub));
			  if(temp[ind] == 0){
				  temp[ind] = 1; 
			  }
      }
			/*for (int j = 0; j < v_size; j++)
			{
				if (sub == vocabulary.at(j))
					temp.push_back(1);
				else
					temp.push_back(0);
			}*/
			//temp.push_back(class_label);
		} while (iss);
   
		temp[v_size] = class_label;
    //cout << "class label: " << temp[v_size] << endl; 
		
		features.push_back(temp);
	}
	
  cout << "Making lookup table for class label predictions" << endl;
 
  vector<int> prob_key1;
  prob_key1.resize(v_size*2,0);
  vector<int> prob_key0;
  prob_key0.resize(v_size*2,0);
  int num_class_0 = 0;
  int num_class_1 = 0;
  fillProbKey1(prob_key1, features, end_of_training, v_size, num_class_0, num_class_1);
  fillProbKey0(prob_key0, features, end_of_training, v_size);
  
  cout << num_class_0 << " " << num_class_1 << " " << end_of_training << endl;
 
	cout << "Generating predicted class labels." << endl;
	//generated class labels for features 6:05:05 6:06:25 6:08:50 6:11:10
	for (int i = end_of_training+1; i < features.size(); i++)
	{
    long double prob1 = probabilityOfRecordsWithClassAndSentence(1, features.at(i), prob_key1, prob_key0, end_of_training, num_class_1, v_size); // WHY?!?!?!?!
    long double prob0 = probabilityOfRecordsWithClassAndSentence(0, features.at(i), prob_key1, prob_key0, end_of_training, num_class_0, v_size);
    cout << prob1 << " " << prob0 << endl;
		if (prob1 > prob0) {
			generated_class_labels.push_back(1);
		} else {
			generated_class_labels.push_back(0);
		}
		//cout << "Class " << i+1 << " of " << features.size() << " done." << endl;
	}
	
	cout << "Printing results." <<  endl;
	ofstream output_train, output_test;
	output_train.open("preprocessed_train.txt");
	output_test.open("preprocessed_test.txt");
	
	//print results
	set<string>::iterator it;
	for (it = vocabulary.begin(); it!=vocabulary.end(); it++)
	{
		output_train << *it << ", ";
		output_test << *it << ", ";
	}
	output_train << "classlabel" << endl;
	output_test << "classlabel" << endl;
	for (int i = 0; i < end_of_training; i++)
	{
    
    int j = 0;
    int max = features.at(i).size();
		while (j < max)
		{ 
      	//cout <<j << ", " << features.at(i).size() << ", " << (j < features.at(i).size()) << " ";
			  if (j == (max - 1)){
				  output_train << features.at(i).at(j);
      	}
		  	else{
				  output_train << features.at(i).at(j) << ", ";
      	}
      	j++; 
		}
    //cout << endl;
		output_train << endl;
	}
	for (int i = end_of_training + 1; i < features.size(); i++)
	{
		for (int j = 0; j < features.at(i).size() - 1; j++)
		{
			if (j == features.at(i).size() - 2)
				output_test << generated_class_labels.at(((i-end_of_training)-1));
			else
				output_test << features.at(i).at(j) << ", ";
		}
		output_test << endl;
	}
	
	long double correct_predictions = 0.0;
	for (int i = 0; i < generated_class_labels.size(); i++)
	{
		if (generated_class_labels.at(i) == features.at(i+end_of_training).at(features.at(i+end_of_training).size() - 1))
			correct_predictions++;
	}
	
	
	ofstream results;
	results.open("results.txt", ofstream::app);
	results << "Training file: " << argv[1] << endl;
	results << "Testing file: " << argv[2] << endl;
	results << "Number of correct predictions: " << correct_predictions << endl;
	results << "Percent accuracy: " << (correct_predictions/generated_class_labels.size())*100 << "%"<< endl << endl << endl;
	
	training_data.close();
	testing_data.close();
	output_test.close();
	output_train.close();
	results.close();
	
	
	return 0;
}









