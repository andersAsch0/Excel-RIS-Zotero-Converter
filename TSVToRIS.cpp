#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::string;
using std::min;

//THIS DOES NOT MAKE "REAL" RIS FILES. I am not fully following the RIS rules because Zotero does not require it in order to import.
//ex. I have not formatted the dates in the DA field in the proper RIS fashion because it would be a pain and Zotero doesn't need it. 

// NO file extensions
const string INPUTFILENAME = "test"; //replace with your file name (no file extension. it should be a tsv)
const string OUTPUTFILENAME = "RISOutput";

string makeRISTag(string tag){
    if(tag.length() != 2) cout << "ERROR! Tag must be length 2";
    return tag+"  - ";
}

string addToNotes (string currNotes, string label, string cell){
    if(cell == "") return currNotes;
    string newNotes = currNotes;
    if(newNotes.length() != 0) newNotes = newNotes + "\n"; //new line for new note 
    newNotes = newNotes+makeRISTag("N1")+label+": "+cell;
    return newNotes;
}

string addToLocation (string currLocation, string label, string cell){
    string newLoc= currLocation;
    if(newLoc.length() == 0) newLoc = makeRISTag("AV");
    else newLoc = newLoc+", ";
    newLoc = newLoc+label+": "+cell;
    return newLoc;
}

string getTitle(string cell){
    return makeRISTag("T1")+cell;
}

string getAuthors(string cell){
    string delimiters[] = {";",",","and","&"}; //extract each separate author from a string which contains all of them, separated by any of these delimiters
    int numDelimiters = 4;
    string delimiter, author;
    size_t authorEndPos = 0;
    size_t prevAuthorEndPos = 0;
    size_t delimiterPos;
    string authors = "";
    while(true){
        delimiter = "";
        authorEndPos = cell.find(",", prevAuthorEndPos);
        for(int i = 0; i < numDelimiters; i++){ //check each potential delimiter and find the next after the previous author we already found
            delimiterPos = cell.find(delimiters[i], prevAuthorEndPos);
            if(delimiterPos <= authorEndPos) {
                authorEndPos = delimiterPos;
                delimiter = delimiters[i];
            }
        }
        author = cell.substr(prevAuthorEndPos, authorEndPos-prevAuthorEndPos);
  
        if(authors.length()!=0) authors = authors+"\n"; //newline if there are multiple authors, this one needs a new line
        authors = authors+makeRISTag("AU")+author; //add the newly discovered author
        if( authorEndPos == string::npos) break; //if weve reached the end stop
        authorEndPos += delimiter.length(); //trim off delimiter for next search

    
        prevAuthorEndPos = authorEndPos;
        
    }
    return authors; //dont have to worry about weird whitespace or name order because Zotero takes care of it!!!!!!!! Joyous day
}

string getCategory(string cell){
    return makeRISTag("KW")+cell;
}

string getType(string cell, string formatCell){ //hard coded for what was on the spreadsheet
    string type = "GEN";
    if(cell=="Book") type = "BOOK";
    else if(cell=="Journal") type = "JFULL";
    else if(cell=="Report") type = "RPRT";
    else if(cell=="Magazine") type = "MGZN";
    else if(cell=="Bulletin") {
        if (formatCell=="Pam") type = "PAMP";
        else type = "RPRT";
    }
    return makeRISTag("TY")+type;
     
}

string getEdition(string cell){
    return makeRISTag("ET")+cell;
}

string getVolume(string cell){
    return makeRISTag("VL")+cell;
}

string getFormat(string cell){ //hard coded for what was on the spreadsheet
    string format = cell;
    if(cell=="Pb") format = "Paperback";
    else if(cell=="H") format = "Hardcover";
    else if(cell=="Pam") format = "Pamphlet";
    else if(cell=="OTH") format = "";
    return format; //no ris tag because this is going in notes
}

string getMarkup(string cell){
    if(cell.length()>=4) return cell.substr(4); //cut off the "MU: " in front
    else return cell; //no ris tag because this is going in notes
}

string getNumber(string cell){
    return makeRISTag("IS")+cell;
}

string getDate(string cell){
    return makeRISTag("DA")+cell; //THIS IS NOT PROPER RIS DATE FORMAT! but Zotero takes it np so Im going to leave it at that
}

string getPublication(string cell){
    return makeRISTag("T2")+cell;
}


int main(){
    


    std::ifstream TSVFile;
    TSVFile.open(INPUTFILENAME+".tsv", std::ifstream::in);
    std::ofstream RISFile;
    RISFile.open(OUTPUTFILENAME + ".txt"); //clears / overwrites file since its not in append mode
    if(!RISFile.is_open()){
        cout << "Error: could not open or create output file." << endl;
        return 0;
    }

    string cell;
    string notes, authors, title, category, type, edition, volume, number, year, date, publication, location, keyword2;
    std::getline(TSVFile, cell); //remove first row which is the labels


    while(!TSVFile.eof()) {
        cell = "";
        while(cell.find_first_not_of(" \t\n\v\f\r") == std::string::npos) { std::getline(TSVFile, cell, '\t'); } //skip thru whitespaces until you get to next row 
        //go through each cell of this row and extract each one
        location = addToLocation("", "Box", cell);
        std::getline(TSVFile, cell, '\t'); // Name
        title = getTitle(cell);
        if(title == "") break; //stop program if you reach a line without a title because its not a valid row, probably the end of the spreadsheet
        std::getline(TSVFile, cell, '\t'); // author last. dont care
        std::getline(TSVFile, cell, '\t'); // list of author full names
        authors = getAuthors(cell);
        std::getline(TSVFile, cell, '\t'); // Category
        category = getCategory(cell); 
        std::getline(TSVFile, cell, '\t'); // Type
        type = cell; //store until I have the format
        std::getline(TSVFile, cell, '\t'); // Edition
        edition = getEdition(cell);
        std::getline(TSVFile, cell, '\t'); // Volume
        volume = getVolume(cell);
        std::getline(TSVFile, cell, '\t'); // Format
        type = getType(type, cell);
        notes = addToNotes(notes, "Format", getFormat(cell));
        std::getline(TSVFile, cell, '\t'); // Markup
        notes = addToNotes(notes, "Markup", getMarkup(cell));
        std::getline(TSVFile, cell, '\t'); // Condition
        notes = addToNotes(notes, "Condition", cell);
        std::getline(TSVFile, cell, '\t'); // Notes
        notes = addToNotes(notes, "Notes", cell);
        std::getline(TSVFile, cell, '\t'); // Number
        number = getNumber(cell);
        std::getline(TSVFile, cell, '\t'); // Date
        date = getDate(cell);
        std::getline(TSVFile, cell, '\t'); // Publication
        publication = getPublication(cell);
        std::getline(TSVFile, cell, '\t'); // Unit only. Dont care
        std::getline(TSVFile, cell, '\t'); // Unit and Number
        location = addToLocation(location, "Unit", cell);
        std::getline(TSVFile, cell, '\t'); // Old Sub Category
        keyword2 = getCategory(cell);
        std::getline(TSVFile, cell, '\t'); // Old Box




        //add all the formatted data to the output file in the correct order
        RISFile << type+"\n";
        RISFile << title+"\n";
        RISFile << authors+"\n";
        RISFile << category+"\n";
        RISFile << keyword2+"\n";
        RISFile << edition+"\n";
        RISFile << volume+"\n";
        RISFile << number+"\n";
        RISFile << publication+"\n";
        RISFile << date+"\n";
        RISFile << notes+"\n";
        RISFile << location+"\n";
        RISFile << makeRISTag("ER")+"\n";


        notes = ""; //clear for next round
        

    }





    RISFile.close();
    TSVFile.close();
    
}