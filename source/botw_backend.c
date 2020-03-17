#include "botw_backend.h"

int header[15]     = {0x24e2, 0x24ee, 0x2588, 0x29c0, 0x2a46,  0x2f8e,  0x3ef8,  0x3ef9, 0x471a,  0x471b, 0x471b,  0x471e, 0x0f423d, 0x0f423e,0x0f423f};
long int hashes[] = {
    0x0cbf052a,
    0x1e3fd294,
    0x23149bf8,
    0x57ee221d,
    0x5f283289,
    0x69f17e8a,
    0x6a09fc59,
    0xa6d926bc,
    0xc5238d2b
};

void writeFile(){

    if(rupeeValue != rupees){
            fseek(fp,locations[RUPEES], SEEK_SET);
            fwrite(&rupeeValue, sizeof(long int), 1, fp);
    }

    for(int x = 0; x < numberOfItems; x++){
        if(newQuantItems[x] != quantItems[x]){
            fseek(fp, locations[ITEMS_QUANTITY] + (8 * x),SEEK_SET);
            fwrite(&newQuantItems[x], sizeof(int), 1, fp);
        }
    }

    for(int x = 0; x<numberOfWeapons; x++){
        if(new_quantMod[x] != quantMod[x]){
            fseek(fp, locations[FLAGSV_WEAPON] + (8 * x),SEEK_SET);
            fwrite(&new_quantMod[x], sizeof(int), 1, fp);
        }

        if(new_modNames[x] != modNames[x]){
            fseek(fp, locations[FLAGS_WEAPON] + (8 * x),SEEK_SET);
            fwrite(&new_modNames[x], sizeof(int), 1, fp);
        }    
    }

    for(int x = 0; x<numberOfBows; x++){
        if(new_quantMod[x + numberOfWeapons] != quantMod[x + numberOfWeapons]){
            fseek(fp, locations[FLAGSV_BOW] + (8 * x),SEEK_SET);
            fwrite(&new_quantMod[x + numberOfWeapons], sizeof(int), 1, fp);
        }
        if(new_modNames[x + numberOfWeapons] != modNames[x + numberOfWeapons]){
            fseek(fp, locations[FLAGS_BOW] + (8 * x),SEEK_SET);
            fwrite(&new_modNames[x + numberOfWeapons], sizeof(int), 1, fp);
        }    
    }

    for(int x = 0; x<numberOfShields; x++){

        if(new_quantMod[x + numberOfWeapons + numberOfBows + 6] != quantMod[x + numberOfWeapons + numberOfBows + 6]){
            fseek(fp, locations[FLAGSV_SHIELD] + (8 * x),SEEK_SET);
            fwrite(&new_quantMod[x + numberOfWeapons + numberOfBows + 6], sizeof(int), 1, fp);
        }
        if(new_modNames[x + numberOfWeapons + numberOfBows + 6] != modNames[x + numberOfWeapons + numberOfBows + 6]){
            fseek(fp, locations[FLAGS_SHIELD] + (8 * x),SEEK_SET);
            fwrite(&new_modNames[x + numberOfWeapons + numberOfBows + 6], sizeof(int), 1, fp);
        }
    }

}

void getOffsets(Mapping mapping){
    unsigned int desiredHash = hashes[mapping];
    long location = ftell(fp);
    unsigned int currentHash = 0;
    while(location < maxSize){
        fread(&currentHash, sizeof currentHash, 1, fp);
        location = ftell(fp);
        fseek(fp, 4, SEEK_CUR);
        if(currentHash == desiredHash){
            break;
        }
    }
    locations[mapping] = location;
}

void getData(){

    int readHeader;
    fseek(fp,0,SEEK_END);
    maxSize = ftell(fp);
    fseek(fp,0,SEEK_SET);
    fread(&readHeader, sizeof(int), 1, fp);
    for(version = 0; version<15; version++){
        if(readHeader == header[version]){
            break;
        }
    }

    getOffsets(FLAGS_BOW);
    getOffsets(FLAGSV_BOW);
    getOffsets(RUPEES);
    getOffsets(ITEMS);
    getOffsets(FLAGSV_SHIELD);
    getOffsets(ITEMS_QUANTITY);
    getOffsets(FLAGSV_WEAPON);
    getOffsets(FLAGS_SHIELD);
    

    // The ID check logic in the next section is wrong for 1.6
    
    fseek(fp,locations[RUPEES],SEEK_SET);
    fread(&rupees, sizeof(long int), 1, fp);
    rupeeValue = rupees;
    int endOfItems = 0;
    
    for(int y = 0; y < 50 ;y++){
        
        int offset = (y * 128);

        for(int x = 0; x < 5; x++){
            char tmpString[5];
            
            fseek(fp, locations[ITEMS] + (8 * x) + offset,SEEK_SET);
            fread(&tmpString, sizeof(int), 1, fp);

            if(tmpString[strlen(tmpString) - 1] == 2){
                tmpString[strlen(tmpString) - 1] = 0;
            }

            sprintf(itemName[y] + strlen(itemName[y]),"%s", tmpString);

            if(strcmp(itemName[y],"Armo") == 0)
                endOfItems = 1;

            if(strcmp(tmpString,"swor")==0 || strcmp(tmpString,"word")==0 || strcmp(tmpString,"pear")==0){
                numberOfWeapons++;
            }

            if(strcmp(tmpString,"ow_0")==0){
                numberOfBows++;
            }
            
            if(strcmp(tmpString,"hiel")==0){
                numberOfShields++;
            }
        }

        if(endOfItems == 1)
            break;

        fseek(fp, locations[ITEMS_QUANTITY] + (8 * y),SEEK_SET);
        fread(&quantItems[y], sizeof(int), 1, fp);

        newQuantItems[y] = quantItems[y];
        numberOfItems++;
    }

    int x;

    for(x = 0; x<numberOfWeapons; x++){
            
        fseek(fp, locations[FLAGS_WEAPON] + (8 * x),SEEK_SET);
        fread(&modNames[x], sizeof(int), 1, fp);
        fseek(fp, locations[FLAGSV_WEAPON] + (8 * x),SEEK_SET);
        fread(&quantMod[x], sizeof(int), 1, fp);
        new_modNames[x] = modNames[x];
        new_quantMod[x] = quantMod[x];
            
    }

    
    for(x = 0; x<numberOfBows; x++){
        fseek(fp, locations[FLAGS_BOW] + (8 * x),SEEK_SET);
        fread(&modNames[x + numberOfWeapons], sizeof(int), 1, fp);
        fseek(fp, locations[FLAGSV_BOW] + (8 * x),SEEK_SET);
        fread(&quantMod[x + numberOfWeapons], sizeof(int), 1, fp);
        new_modNames[x + numberOfWeapons] = modNames[x + numberOfWeapons];
        new_quantMod[x + numberOfWeapons] = quantMod[x + numberOfWeapons];
    }

    for(x = 0; x<numberOfShields; x++){
        fseek(fp, locations[FLAGS_SHIELD] + (8 * x),SEEK_SET);
        fread(&modNames[x + numberOfWeapons + numberOfBows + 6], sizeof(int), 1, fp);
        fseek(fp, locations[FLAGSV_SHIELD] + (8 * x),SEEK_SET);
        fread(&quantMod[x + numberOfWeapons + numberOfBows + 6], sizeof(int), 1, fp);
        new_modNames[x + numberOfWeapons + numberOfBows + 6] = modNames[x + numberOfWeapons + numberOfBows + 6];
        new_quantMod[x + numberOfWeapons + numberOfBows + 6] = quantMod[x + numberOfWeapons + numberOfBows + 6];
    }

}


int setFile(int intSlot){

	for (int x = 0; x < 60; x++){
		itemName[x][0] = 0;
		quantItems[x] = 0;
		newQuantItems[x] = 0;	
    }
    

	numberOfItems = 0; 
	rupeeValue = 0;
	rupees = 0;

    char file_name[] = "save:/0/game_data.sav";
    file_name[6] = (char)(intSlot + 48);
    fp = fopen(file_name,"r+b");
    
    if( fp == NULL )
    {
        return 0;
    } else {
        getData(); 
        return 1; 
    }
    
}

