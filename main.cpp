#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <thread>
#include <regex>
#include <mutex>
#include <set>
#include <boost/algorithm/string.hpp>


using namespace std;

string file_path = "/home/angellogan/CV_CPP_prac/projects_folder/Intent_Recognizer_Mbition/data.txt";

static std::unordered_multimap<string,string> map_of_class;
std::set<std::string> set_of_stop_words;
std::set<std::string> query_string;
std::vector<std::string> string_keywords;
//std::vector<std::string> suffixes{"ily", "ted","ed","y", "ing", "ty"};
std::mutex mtx;
std::vector<std::string> labels{"weather","calendar","city","fact"};
std::string intention;







void intent_classifier()
{

//std::cout<<"======================================= INTENT CLASSIFER======================================="<<endl;




    std::thread weather_thread([](){


   //     cout<<"Executing inside thread"<<endl;

        int count=0;

        auto range_weather = map_of_class.equal_range("weather");

        for(auto it2= string_keywords.begin();it2!=string_keywords.end();++it2)
        {

            for(auto it=range_weather.first;it!=range_weather.second;++it)
                 {


                     std::string str1 = it->second;
                     std::string str2 = *it2;

                    if(std::regex_search(str2,regex(str1)))
                    {

                        ++count;

                     }




             }


        }
  //cout<<"weather Count = "<<count<<endl;

        if(count >0)
        {

            mtx.lock();
           intention.append(" weather ");
           mtx.unlock();
        }






    });




    std::thread calendar_thread([](){

      //  cout<<"Executing inside thread"<<endl;



        int count=0;

        auto range_weather = map_of_class.equal_range("calendar");


        for(auto it2= string_keywords.begin();it2!=string_keywords.end();++it2)
        {
            for(auto it=range_weather.first;it!=range_weather.second;++it)
            {



                 std::string str1 = it->second;
                 std::string str2 = *it2;


                 if(std::regex_search(str2,regex(str1)))
                 {

                     ++count;

                  }


                }


           }

      //  cout<<"Calendar Count = "<<count<<endl;



        if(count >0)
        {
             mtx.lock();


           intention.append(" calendar ");
            mtx.unlock();
        }





    });
    std::thread city_thread([](){

       // cout<<"Executing inside thread"<<endl;


        int count=0;

        auto range_weather = map_of_class.equal_range("city");


        for(auto it2= string_keywords.begin();it2!=string_keywords.end();++it2)
        {

            for(auto it=range_weather.first;it!=range_weather.second;++it)
              {



                    std::string str1 = it->second;
                    std::string str2 = *it2;



                    if(std::regex_search(str2,regex(str1)))
                    {

                        ++count;

                     }


               }


        }

     //   cout<<"city Count = "<<count<<endl;

        if(count >0)
        {
             mtx.lock();
           intention.append(" city ");
            mtx.unlock();
        }



});
    std::thread fact_thread([](){

       // cout<<"Executing inside thread"<<endl;

      //  cout<<map_of_class.count("weather")<<endl;

        int count=0;

        auto range_weather = map_of_class.equal_range("fact");


        for(auto it2= string_keywords.begin();it2!=string_keywords.end();++it2)
        {

            for(auto it=range_weather.first;it!=range_weather.second;++it)
                {


                    std::string str1 = it->second;
                     std::string str2 = *it2;


                     if(std::regex_search(str2,regex(str1)))
                     {

                         ++count;

                      }

                 }


        }

        //cout<<"fact Count = "<<count<<endl;

        if(count > 0)
        {
             mtx.lock();
           intention.append(" fact ");
            mtx.unlock();
        }



});






    weather_thread.join();
    calendar_thread.join();
    city_thread.join();
    fact_thread.join();


}

void remove_stop_words(string query)
{


  //  cout<<"Inside the Keywords function"<<endl;


    string line;

    ifstream my_stop_word_file;
    my_stop_word_file.open("/home/angellogan/CV_CPP_prac/projects_folder/Intent_Recognizer_Mbition/stop_words.txt");

    if(!my_stop_word_file)
        {
       cerr << "Error: " << strerror(errno);


       cout<<"Error!!  could not open the file"<<endl;
        }


  while(getline(my_stop_word_file,line))
     {

     set_of_stop_words.insert(line);



    }







  stringstream ss(query);
  string s;

  while(getline(ss,s,' '))
  {

      query_string.insert(s);

  }

   // cout<<query_string.size()<<endl;



    string_keywords.reserve(100);

/* to remove the stop words and to obtain bag of keywords*/

   std::set_difference(query_string.begin(),query_string.end(),set_of_stop_words.begin(),set_of_stop_words.end(),std::inserter(string_keywords, string_keywords.begin()));




    string_keywords.shrink_to_fit();

   // cout<<string_keywords.size()<<endl;

    /*
    for(auto word:string_keywords)
    {

    cout<<word<<endl;

    }

*/




     intent_classifier();

     if(intention.empty())
     {
        cout<<"Intention is Unknown.. Please check the input again"<<endl;

     }

     else  if(intention.find("calendar")!=string::npos){



         cout<<"Intent: Check "<<intention<<endl;




     }
     else

     {


         cout<<"Intent : Get "<<intention<<endl;

     }



     intention.clear();
}


int main()
{
    cout << "!!!!!!!!!!!!!!!!!!!!   Intent Recognizer !!!!!!!!!!!!!!!!" << endl<<endl<<endl;

    string key;
    string value;
    string line;
    std::vector<std::string> labels;
    string delimiter = ":";


    ifstream my_input_file;

    my_input_file.open("/home/angellogan/CV_CPP_prac/projects_folder/Intent_Recognizer_Mbition/data.txt");


    if(!my_input_file)
    {
        cerr << "Error: " << strerror(errno);


        cout<<"Error!!  could not open the file"<<endl;
    }


   while(getline(my_input_file,line))
   {

        string first = line.substr(0,line.find(delimiter));
        string second = line.substr(line.find(delimiter) + 1 ,line.find('\n'));

        map_of_class.insert(std::make_pair(first,second));

   }



   //cout<<map_of_class.count("weather")<<endl;

  // std::cout<<map_of_class.size()<<endl;


    string choice;

    do{

        std::cout<<"Enter your query please"<<endl;
        getline(cin,value);


        boost::to_lower(value);

       // cout<<value<<endl;



        std::thread th1(&remove_stop_words,value);



        th1.join();

        std::cout<<"Do you have another query?If yes : enter \"y\" else enter \"n\""<<endl;

        getline(cin,choice);
        boost::to_lower(choice);


        if(choice=="y")
        {

            query_string.clear();
             intention.clear();
             string_keywords.clear();

        }


        else if(choice=="n")
        {


            exit(0);
        }

        else
        {

            cout<<"Invalid entry... Please enter valid input"<<endl;

        }



    }while(choice!="n");




    return 0;
}
