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

float probabilityOfRecordsWithClassAndSentence(int _class, vector<int> sentence, vector<vector<int> > training_data, int end_of_training)
{
	float probability = 1;
	int records_with_both = 0;
	int records_with_one = 0;
	int class_count = 0;
	vector<int> temp = sentence;
	temp.at(temp.size() - 1) = _class;
	for (int i = 0; i < end_of_training; i++)
	{
		records_with_both = 0;
		records_with_one = 0;
		for (int j = 0; j < training_data.at(i).size() - 1; j++)
		{
			if (temp.at(j) == training_data.at(i).at(j))
			{
				records_with_one++;
				if (temp.at(temp.size() - 1) == training_data.at(i).at(training_data.size() - 1))
					records_with_both++;
			}
		}
		if (_class = training_data.at(i).at(training_data.size() - 1))
			class_count++;
		probability += log(((records_with_both + 1) / (records_with_one + 2)));
	}
	probability += log((class_count / end_of_training + 1));
	
	
	return probability;
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
    if(line_no_punct[line_no_punct.size()-1] == '0'){
      class_label = 0;
    }
    else{
      class_label = 1;
    }
    line_no_punct[line_no_punct.size()-1] = ' ';
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
    if(line_no_punct[line_no_punct.size()-1] == '0'){
      class_label = 0;
    }
    else{
      class_label = 1;
    }
    line_no_punct[line_no_punct.size()-1] = ' ';
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

		end_of_training++;
		features.push_back(temp);
	}
	
	cout << "Generating predicted class labels." << endl;
	//generated class labels for features 6:05:05 6:06:25 6:08:50 6:11:10
	for (int i = 0; i < features.size(); i++)
	{
		if (probabilityOfRecordsWithClassAndSentence(1, features.at(i), features, end_of_training) >
		probabilityOfRecordsWithClassAndSentence(0, features.at(i), features, end_of_training)) {
			generated_class_labels.push_back(1);
		} else {
			generated_class_labels.push_back(0);
		}
		cout << "Feature " << i+1 << " of " << features.size() << " done." << endl;
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
      	cout <<j << ", " << features.at(i).size() << ", " << (j < features.at(i).size()) << " ";
			if (j = features.at(i).size() - 1){
				output_train << features.at(i).at(j);
      	}
			else{
				output_train << features.at(i).at(j) << ", ";
      	}
      	j++; 
		}
    cout << endl;
		output_train << endl;
	}
	for (int i = 0 + 1; i < features.size(); i++)
	{
		for (int j = 0; j < features.at(i).size() - 1; j++)
		{
			if (j = features.at(i).size() - 2)
				output_test << generated_class_labels.at(i);
			else
				output_test << features.at(i).at(j) << ", ";
		}
		output_test << endl;
	}
	
	int correct_predictions = 0;
	for (int i = 0; i < generated_class_labels.size(); i++)
	{
		if (generated_class_labels.at(i) == features.at(i).at(features.at(i).size() - 1))
			correct_predictions++;
	}
	
	
	ofstream results;
	results.open("results.txt", ofstream::app);
	results << "Training file: " << argv[1] << endl;
	results << "Testing file: " << argv[2] << endl;
	results << "Number of correct predictions: " << correct_predictions << endl;
	results << "Percent accuracy: " << (float)(correct_predictions/generated_class_labels.size())*100 << "%"<< endl << endl << endl;
	
	training_data.close();
	testing_data.close();
	output_test.close();
	output_train.close();
	results.close();
	
	
	return 0;
}









