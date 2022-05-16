/* program_4.cpp
    Divvy Bike Data Analysis program.
    Date: 11/3/2021
    System: XCode using macOS
*/

#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <cctype>     // access to tolower() or toupper() function
#include <vector>     // For vectors
#include <string>     // String operations
#include <algorithm>  // use of vector algorithms
#include <cmath>        // for math functions
#include <iomanip>      //for setw

using namespace std;

//A STRUCT NAMED DIVVY THAT HAS GETTERS AND SETTERS REQUIRED FOR THIS STRUCT
struct Divvy{
    void SetDetails(string rideID, string rideableType, string startedAt, string endedAt, string startStationName, string endStationName, long double startLat, long double startLng, long double endLat, long double endLng, string memberCasual, int day, int hour){
        this->rideID.push_back(rideID);
        this->rideableType.push_back(rideableType);
        this->startedAt.push_back(startedAt);
        this->endedAt.push_back(endedAt);
        this->startStationName.push_back(startStationName);
        this->endStationName.push_back(endStationName);
        this->startLat.push_back(startLat);
        this->startLng.push_back(startLng);
        this->endLat.push_back(endLat);
        this->endLng.push_back(endLng);
        this->memberCasual.push_back(memberCasual);
        this->day.push_back(day);
        this->hour.push_back(hour);
    }
    vector <string> rideID, rideableType, startedAt, endedAt, startStationName, endStationName, memberCasual;
    vector <double>  startLat, startLng, endLat, endLng ;
    vector <int> day, hour;
    double perHourWeekday[24], perHourWeekend[24], maxValue;
};

//GLOBAL INITIALIZATION OF SOME VALUES AS IT IS USED EVERYWHERE IN THE PROGRAM
Divvy SampleData, WeekendWeekdayData;
int ChoiceOption1=0;
long int size1, size2;

// Utility functions getting distant between two map points
// Attribution: https://www.geeksforgeeks.org/program-distance-two-points-earth/
// converting degrees to radians

//FUNCTION TO CONVERT DEGREE TO RADIANS
long double toRadians(const long double degree)
{
    // cmath library in C++
    // defines the constant
    // M_PI as the value of
    // pi accurate to 1e-30
    long double one_deg = (3.14159265358979323846) / 180;
    return (one_deg * degree);
}

//FUNCTION TO CALCULATE DISTANCE USING LATITIUDES AND LONGTITUDES
long double distance(long double lat1, long double long1,
    long double lat2, long double long2)
{
    // Convert the latitudes
    // and longitudes
    // from degree to radians.
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);

    // Haversine Formula
    long double dlong = long2 - long1;
    long double dlat = lat2 - lat1;

    long double ans = pow(sin(dlat / 2), 2) +
        cos(lat1) * cos(lat2) *
        pow(sin(dlong / 2), 2);

    ans = 2 * asin(sqrt(ans));

    // Radius of Earth in
    // Kilometers, R = 6371
    // Use R = 3956 for miles
    long double R = 3956;

    // Calculate the result
    ans = ans * R;
    return ans;
}

//FUNCTION TO MOST NO OF RIDES BY HR IN SAMPLE DATA
void largestValueSampleData(){
    for(int i=0 ;i<24 ; i++){
        if(SampleData.maxValue < SampleData.perHourWeekday[i]){
            SampleData.maxValue =SampleData.perHourWeekday[i];
        }
        else if(SampleData.maxValue < SampleData.perHourWeekend[i]){
            SampleData.maxValue =SampleData.perHourWeekend[i];
        }
    }
}

//FUNCTION TO CALCULATE MOST NO OF RIDES BY HR IN WEEKEND WEEKDAY DATA
void largestValueWeekendWeekdayData(){
    for(int i=0 ;i<24 ; i++){
        if(WeekendWeekdayData.maxValue < WeekendWeekdayData.perHourWeekday[i]){
            WeekendWeekdayData.maxValue =WeekendWeekdayData.perHourWeekday[i];
        }
        else if(WeekendWeekdayData.maxValue <WeekendWeekdayData.perHourWeekend[i]){
            WeekendWeekdayData.maxValue =WeekendWeekdayData.perHourWeekend[i];
        }
    }
}

//FUNCTION TO CONVERT STRING TO INT
void convertStringToInt(string startLat, string startLng, string endLat, string endLng,long double &beginLat, long double &beginLng, long double &finishLat, long double &finishLng){

    beginLat = stold(startLat);
    beginLng = stold(startLng);
    finishLat = stold(endLat);
    finishLng = stold(endLng);
}

//FUNCTION TO SPLIT DATE INTO DAY AND HR
void splitStringDate(string startedAt, int &dt, int &hr){
    unsigned long int found, found1, found2, found3;
    //finding the day and hr using the 'find 'function and storing in integer value by converting it using 'stoi'
    found = startedAt.find('/');
    found1 = startedAt.find('/', found+1);
    found2 = startedAt.find(' ', found1+1);
    found3 = startedAt.find(':', found2+1);
    dt = stoi(startedAt.substr(found+1,found1));
    hr = stoi(startedAt.substr(found2+1,found3));
}

//FUNCTION TO CALCULATE NUMBER OF RIDES ON WEEKEND AND WEEKDAY
void countWeekdayWeekend(int day, int hour, double perHourWeekDay[24], double perHourWeekEnd[24]){
    //check if the day is weekend or weekday and count accordingly
    if( day == 1 ){
        perHourWeekDay[hour]++;
    }
    else if( day == 4){
        perHourWeekEnd[hour]++;
    }
}

//FUNCTION TO READ SAMPLE DATA INTO A VECTOR FROM A CSV FILE
void readWordsIntoSampleData(int &CleanData, int &TotalData )
{
    int uncleanData = 0, Day = 0, Hour = 0;
    ifstream sample("divvyridesampledata.csv");                      // Declare an input stream for reading
    assert( sample.fail() == false );     // Ensure file open worked
    string rideid, rideabletype, startedat, endedat, startstationname, endstationname, startlat, startlng, endlat, endlng, memberCasual;
    long double StartLat, StartLng, EndLat, EndLng;
//    ignore the first line in data
    getline(sample, memberCasual, '\r');
    while(sample.good()){
        getline(sample, rideid, ',');
        getline(sample, rideabletype, ',');
        getline(sample, startedat, ',');
        getline(sample, endedat, ',');
        getline(sample, startstationname, ',');
        getline(sample, endstationname, ',');
        getline(sample, startlat, ',');
        getline(sample, startlng, ',');
        getline(sample, endlat, ',');
        getline(sample, endlng, ',');
        getline(sample, memberCasual, '\r');
        if(TotalData==13){break;}
        ++TotalData;
        //clean the data
        if(rideid=="" || rideabletype=="" || startedat=="" || endedat=="" || startstationname=="" || endstationname=="" || startlat=="" || startlng=="" || endlat=="" || endlng == "" || memberCasual ==""){uncleanData++;}
        //store the clean data
        else{
        convertStringToInt(startlat, startlng, endlat, endlng, StartLat, StartLng, EndLat, EndLng);
        splitStringDate(startedat, Day, Hour);
        SampleData.SetDetails(rideid, rideabletype, startedat, endedat, startstationname, endstationname, StartLat, StartLng, EndLat, EndLng, memberCasual, Day, Hour);
            countWeekdayWeekend(Day, Hour, SampleData.perHourWeekday, SampleData.perHourWeekend);
            largestValueSampleData();
        }

}
    CleanData = TotalData - uncleanData;
    sample.close();
}

//FUNCTION TO READ WEEKEND WEEKDAY DATA INTO VECTOR FROM A CSV FILE
void readWordsIntoWeekendAndWeekdayData(int &CleanData, int &TotalData)
{
    int uncleanData = 0, Day = 0, Hour = 0;
    ifstream DaysWeekData("weekdayweekend.csv");                      // Declare an input stream for reading
    assert( DaysWeekData.fail() == false );     // Ensure file open worked
    string rideid, rideabletype, startedat, endedat, startstationname, endstationname, startlat, startlng, endlat, endlng, memberCasual;
    long double StartLat, StartLng, EndLat, EndLng;
//    ignore the first line in data
    getline(DaysWeekData, memberCasual, '\r');
    while(DaysWeekData.good()){
        getline(DaysWeekData, rideid, ',');
        getline(DaysWeekData, rideabletype, ',');
        getline(DaysWeekData, startedat, ',');
        getline(DaysWeekData, endedat, ',');
        getline(DaysWeekData, startstationname, ',');
        getline(DaysWeekData, endstationname, ',');
        getline(DaysWeekData, startlat, ',');
        getline(DaysWeekData, startlng, ',');
        getline(DaysWeekData, endlat, ',');
        getline(DaysWeekData, endlng, ',');
        getline(DaysWeekData, memberCasual, '\r');
        if(TotalData==25887){break;}
        ++TotalData;
        //clean the data
        if(rideid=="" || rideabletype=="" || startedat=="" || endedat=="" || startstationname=="" || endstationname=="" || startlat=="" || startlng=="" || endlat=="" || endlng == "" || memberCasual ==""){uncleanData++;}
        //store the clean data
        else{
            convertStringToInt(startlat, startlng, endlat, endlng, StartLat, StartLng, EndLat, EndLng);
            splitStringDate(startedat, Day, Hour);
            WeekendWeekdayData.SetDetails(rideid, rideabletype, startedat, endedat, startstationname, endstationname, StartLat, StartLng, EndLat, EndLng, memberCasual, Day, Hour);
            countWeekdayWeekend(Day, Hour, WeekendWeekdayData.perHourWeekday, WeekendWeekdayData.perHourWeekend);
            largestValueWeekendWeekdayData();
        }

}
    CleanData = TotalData - uncleanData;
//    TotalData-=1;
    DaysWeekData.close();
}
//if user chooses option 1
void OptionOne(){
    int CleanData = 0, TotalData = 0;
    cout<<"Select datafile:"<<endl
        <<"1. Small subset of data with 14 rides to help create your program"<<endl
        <<"2. Week day vs weekend rides"<<endl
        <<"3. All September 2021 data (not tested in Zybooks)"<<endl
    <<"Your selection-->";
    cin>>ChoiceOption1;

//    a switch function to check which file to access
    switch(ChoiceOption1){
//      display the total data and clean data in sample file
        case 1:
            readWordsIntoSampleData(CleanData, TotalData);
            size1 = SampleData.rideID.size();
            cout<<"Total # of trips found in datafile:  "<<TotalData<<endl
                <<"Total # of trips in clean data:  "<<CleanData<<endl;
            break;
//      display the total and clean dat in weekend weekday file
        case 2:
            readWordsIntoWeekendAndWeekdayData(CleanData, TotalData);
            size2 = WeekendWeekdayData.rideID.size();
            cout<<"Total # of trips found in datafile:  "<<TotalData<<endl
                <<"Total # of trips in clean data:  "<<CleanData<<endl;
            break;
    }

}
//if user chooses option 2 : displays the total miles travelled and the longest journey
void OptionTwo(){
    long double TotalDistance = 0, average = 0, MaxDistance = 0, roundTotal = 0;
    string tripID, tripstartLocation, tripendLocation;

//  a switch function to choose which file to access
    switch(ChoiceOption1){
        case 1:
//          a for loop to check the longest trip
            for(int i = 0; i<size1; i++)
            {
                double Distance = distance(SampleData.startLat.at(i), SampleData.startLng.at(i), SampleData.endLat.at(i), SampleData.endLng.at(i));
                if(Distance>MaxDistance){
                    MaxDistance = Distance;
                    tripID = SampleData.rideID.at(i);
                    tripstartLocation = SampleData.startStationName.at(i);
                    tripendLocation = SampleData.endStationName.at(i);
                }
                TotalDistance += Distance;
            }
            roundTotal = round(TotalDistance);
            average = (roundTotal/size1);
            break;

        case 2:
//          a for loop to check the longest trip
            for(int i = 0; i<size2; i++)
            {
                double Distance = distance(WeekendWeekdayData.startLat.at(i), WeekendWeekdayData.startLng.at(i), WeekendWeekdayData.endLat.at(i), WeekendWeekdayData.endLng.at(i));
                if(Distance>MaxDistance){
                    MaxDistance = Distance;
                    tripID = WeekendWeekdayData.rideID.at(i);
                    tripstartLocation = WeekendWeekdayData.startStationName.at(i);
                    tripendLocation = WeekendWeekdayData.endStationName.at(i);
                }
                TotalDistance += Distance;
            }

            roundTotal = round(TotalDistance);
            average = (roundTotal/size2);
            break;

    }



    cout<<"Total # of miles traveled: "<<roundTotal<<endl
    <<"Average length of trips in miles:  "<<fixed<<setprecision(1)<<average<<endl<<endl;

    cout<<"Longest trip information below:"<<endl
        <<"-------------------------------"<<endl
        <<"Trip ID: "<<tripID<<endl
        <<"Trip start location: "<<tripstartLocation<<endl
        <<"Trip end location: "<<tripendLocation<<endl
        <<"Trip distance in miles: "<<fixed<<setprecision(1)<<MaxDistance<<endl;


}
//if user chooses option 3: display the percentage of casual users and members
void OptionThree(){
    double percentageMember = 0, percentageCasual = 0, memberNo = 0, casualNo = 0;
    // A switch function to choose which file to access
    switch(ChoiceOption1){
        case 1:
//          a loop to store number of members and casual users for sample Data
            for(int i = 0 ; i< size1; ++i){
                if(SampleData.memberCasual.at(i)=="member"){
                    memberNo++;
                }
                else if(SampleData.memberCasual.at(i)=="casual"){
                    casualNo++;
                }
            }
            percentageMember = ((memberNo/size1)*100);
            percentageCasual = ((casualNo/size1)*100);

            break;

        case 2:
//          a loop to store number of members and casual users for WeekendWeekday Data
            for(int i = 0 ; i< size2; ++i){
                if(WeekendWeekdayData.memberCasual.at(i)=="member"){
                    memberNo++;
                }
                else if(WeekendWeekdayData.memberCasual.at(i)=="casual"){
                    casualNo++;
                }
            }
//          Calculate the percentage of member and casual users
            percentageMember = ((memberNo/size2)*100);
            percentageCasual = ((casualNo/size2)*100);

            break;
    }
//    Display the percentage of casual users and memebers
    cout<<"Casual Rider Percentage: "<<fixed<<setprecision(1)<<percentageCasual<<"%"<<endl
        <<"Member Rider Percentage: "<<fixed<<setprecision(1)<<percentageMember<<"%"<<endl;


}
//if user chooses option 4 : Displays pictorial and tabular representation of rides per hr on weekend and weekday
void OptionFour(){
    int choice;
    cout<<"Select type of display:"<<endl
        <<"1. Counts of rides per hour in the day"<<endl
        <<"2. Proportional 50 column graph with @ for weekday and + for weekend"<<endl
        <<"Your selection-->";
    cin>>choice;
    // A switch function to choose which file to access
    switch(ChoiceOption1){
//      SAMPLEDATA
        case 1:
            
//        A switch function to either user pictorial or tabular reprsentation
            switch(choice){
                case 1:
                    cout<<"LargestNumberOfRides is: "<<SampleData.maxValue<<endl
                        <<"Rides per hour for weekday and weekend:"<<endl;
//                  A loop to display number of rides per hr on weekend and weekday
                    for(int i = 0 ; i < 24 ; ++i){
                        cout<<i<<":     "<<SampleData.perHourWeekday[i]<<"      "<<SampleData.perHourWeekend[i]<<endl;
                    }
                    break;

                case 2:
//                  A loop to display number of rides per hr on weekend and weekday pictorially
                    for(int i =0; i<24; ++i){
                        double weekdaychars = floor((SampleData.perHourWeekday[i]/SampleData.maxValue)*50.0);
                        double weekendchars = floor((SampleData.perHourWeekend[i]/SampleData.maxValue)*50.0);
                        cout<<i<<": ";
                        for(int j = 0; j<weekdaychars; ++j){
                            cout<<"@";
                        }
                        cout<<endl<<setw(5);
                        for(int j = 0; j<weekendchars; ++j){
                            cout<<"+";
                        }
                        cout<<endl;
                    }
                    break;
            }
            break;

//      WEEKEND WEEKDAY DATA, LARGER DATASET
        case 2:
//          A switch function to either user pictorial or tabular reprsentation
            switch(choice){
                case 1:
                    cout<<"LargestNumberOfRides is: "<<WeekendWeekdayData.maxValue<<endl
                        <<"Rides per hour for weekday and weekend:"<<endl;
//                  A loop to display number of rides per hr on weekend and weekday
                    for(int i = 0 ; i < 24 ; ++i){
                        cout<<i<<":     "<<WeekendWeekdayData.perHourWeekday[i]<<"      "<<WeekendWeekdayData.perHourWeekend[i]<<endl;
                    }
                    break;

                case 2:
//                  A loop to display number of rides per hr on weekend and weekday pictorially
                    for(int i =0; i<24; ++i){
                        double weekdaychars = floor((WeekendWeekdayData.perHourWeekday[i]/WeekendWeekdayData.maxValue)*50.0);
                        double weekendchars = floor((WeekendWeekdayData.perHourWeekend[i]/WeekendWeekdayData.maxValue)*50.0);
                        cout<<i<<": ";
                        for(int j = 0; j<weekdaychars; ++j){
                            cout<<"@";
                        }
                        cout<<endl<<setw(5);
                        for(int j = 0; j<weekendchars; ++j){
                            cout<<"+";
                        }
                        cout<<endl;
                    }
                    break;
            }
            break;
    }


}
//if user chooses option 5 : Displays the nearest Divvy Station
void OptionFive(){
    
    double userLat, userLng, stationLat=0, stationLng=0, dist, Distance=100;
    string closestStation;
    cout<<"Input latitude and longitude of the home: ";
    cin>>userLat>>userLng;
    cout<<endl<<"You entered: "<<setprecision(3)<<userLat<<" for latitude and "<<setprecision(3)<<userLng<<" for longitude"<<endl;
    //a loop to calculate each distance and display the shortest distance
    for(int i = 0; i<size2; ++i){
//      Calculating the distance using the distance function
        dist = distance(userLat, userLng, WeekendWeekdayData.startLat.at(i), WeekendWeekdayData.startLng.at(i));
        if(Distance>dist){
            Distance = dist ;
            closestStation = WeekendWeekdayData.startStationName.at(i);
            stationLat = WeekendWeekdayData.startLat.at(i);
            stationLng = WeekendWeekdayData.startLng.at(i);
        }
    }
    
//  display the nearest divvy station
    cout<<"Closest Divvy station is: "<<closestStation<<" at "<<stationLat<<", "<<stationLng<<", "<<setprecision(1)<<Distance<<" miles away."<<endl;

}
//THIS IS WERE THE EXECUTION OF THE PROGRAM STARTS
int main()
{
    cout<<"Analyze Divvy bike ride information using September trip data."<<endl
        <<"First select option 1 to read and clean the data."<<endl;
    int choice;
    
    //looping to repeat the program until user selects 6
    do{
    cout<<"Select a menu option:"<<endl
        <<"1. Select datafile, display rides totals and clean data"<<endl
        <<"2. Display overall trip information"<<endl
        <<"3. Display percentage of members vs. casual riders"<<endl
        <<"4. Display weekday vs weekend usage"<<endl
        <<"5. Extra Credit: find closest station"<<endl
        <<"6. Exit"<<endl
        <<"Your choice -->";
    cin>>choice;
        if(choice!=1 && ChoiceOption1 == 0){
            cout<<"First read in data by selecting menu option 1" <<endl;
        }
        else
        {
            switch(choice){
                case 1:
                    OptionOne();            //call OptionOne
                    continue;
                case 2:
                    OptionTwo();            //call OptionTwo
                    continue;
                case 3:
                    OptionThree();          //call OptionThree
                    continue;
                case 4:
                    OptionFour();           //call OptionFour
                    continue;
                case 5:
                    OptionFive();           //call OptionFive
                    continue;
            }
        }
    }while (choice!=6);
    return 0;
}

