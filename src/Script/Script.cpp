#include "Script.h"
#include "ScriptSystem.h"
#include "..\Form\Form.h"
#include "..\LevelEditor.h"
#include "..\Utils\FileMan.h"

C_Script::C_Script(vector<S_ScriptData> n_data, string n_name)
{
    kindName = n_name;
    name = n_name;
    data = n_data;
    vector<S_MAPPOS> tempVecPos;
    S_MAPPOS tempPos;
    tempPos.x = 0;
    tempPos.y = 0;
    int tempInt;
    double tempDouble;

    iGroup = C_ScriptSystem::Inst()->GetActiveGroup();

    for(unsigned int loop1 = 0; loop1 < data.size(); loop1++)
    {
        switch(data[loop1].kind)
        {
            case DOUBLE :
                if(FromString<double>(tempDouble, data[loop1].defaultValue, std::dec))
                    doubleData.push_back(tempDouble);
                else
                    doubleData.push_back(0.0);
                break;
            case INTEGER :
                if(FromString<int>(tempInt, data[loop1].defaultValue, std::dec))
                    intData.push_back(tempInt);
                else
                    intData.push_back(0);
                break;
            case STRING : stringData.push_back(data[loop1].defaultValue); break;
            case TILE_LINK : tileData.push_back(tempVecPos); break;
        }
    }
}

XMLNode * C_Script::CreateXMLTree()
{
    XMLNode * root = new XMLNode(kindName);
    root->AddChild(new XMLNode("name", name));

    XMLNode * dataNode = new XMLNode("data");
    XMLNode * tempNode;

    unsigned int iTileLink = 0;
    unsigned int iInteger = 0;
    unsigned int iDouble = 0;
    unsigned int iString = 0;
    ostringstream buffer;

    for(int loop1 = 0; loop1 < data.size(); loop1++)
    {
        buffer.clear();
        buffer.str("");
        switch(data[loop1].kind)
        {
            case INTEGER :
                buffer << intData[iInteger++];
                dataNode->AddChild(new XMLNode(data[loop1].name, buffer.str()));
                break;
            case DOUBLE :
                buffer << doubleData[iDouble++];
                dataNode->AddChild(new XMLNode(data[loop1].name, buffer.str()));
                break;
            case STRING :
                buffer << stringData[iString++];
                dataNode->AddChild(new XMLNode(data[loop1].name, buffer.str()));
                break;
            case TILE_LINK :
                tempNode = new XMLNode(data[loop1].name);
                buffer << tileData[iTileLink].size();
                tempNode->AddAttribute("count", buffer.str());

                XMLNode *tileNode;

                for(int loop2 = 0; loop2 < tileData[iTileLink].size(); loop2++)
                {
                    tileNode = new XMLNode("tile");
                    buffer.clear();
                    buffer.str("");
                    buffer << tileData[iTileLink][loop2].x;
                    tileNode->AddAttribute("x", buffer.str());
                    buffer.clear();
                    buffer.str("");
                    buffer << tileData[iTileLink][loop2].y;
                    tileNode->AddAttribute("y", buffer.str());

                    tempNode->AddChild(tileNode);
                }
                iTileLink++;

                dataNode->AddChild(tempNode);
                break;
        }
    }

    root->AddChild(dataNode);

    return root;
}

void C_Script::Save(FILE *fw, FILE *fwt)
{
    unsigned int loop1, loop2;
    FileMan::Inst()->SaveInteger(iGroup, fw);
    FileMan::Inst()->SaveString(kindName, fw);
    FileMan::Inst()->SaveString(name, fw);
    FileMan::Inst()->SaveInteger(data.size(), fw);
    for(loop1 = 0; loop1 < data.size(); loop1++)
    {
        fwrite(&data[loop1].kind, sizeof(E_SCRIPTDATAKIND), 1, fw);
        FileMan::Inst()->SaveString(data[loop1].name, fw);
        FileMan::Inst()->SaveString(data[loop1].help, fw);
        FileMan::Inst()->SaveInteger(data[loop1].value[0], fw);
        FileMan::Inst()->SaveInteger(data[loop1].value[1], fw);
        FileMan::Inst()->SaveInteger(data[loop1].value[2], fw);
    }
    FileMan::Inst()->SaveInteger(stringData.size(), fw);
    for(loop1 = 0; loop1 < stringData.size(); loop1++)
    {
        FileMan::Inst()->SaveString(stringData[loop1], fw);
    }
    FileMan::Inst()->SaveInteger(intData.size(), fw);
    for(loop1 = 0; loop1 < intData.size(); loop1++)
    {
        FileMan::Inst()->SaveInteger(intData[loop1], fw);
    }
    FileMan::Inst()->SaveInteger(doubleData.size(), fw);
    for(loop1 = 0; loop1 < doubleData.size(); loop1++)
    {
        FileMan::Inst()->SaveDouble(doubleData[loop1], fw);
    }
    FileMan::Inst()->SaveInteger(tileData.size(), fw);
    for(loop1 = 0; loop1 < tileData.size(); loop1++)
    {
        FileMan::Inst()->SaveInteger(tileData[loop1].size(), fw);
        for(loop2 = 0; loop2 < tileData[loop1].size(); loop2++)
        {
            FileMan::Inst()->SaveInteger(tileData[loop1][loop2].x, fw);
            FileMan::Inst()->SaveInteger(tileData[loop1][loop2].y, fw);
        }
    }

    /* OLD SAVE TEXT SCRIPT
    unsigned int iTileLink = 0;
    unsigned int iInteger = 0;
    unsigned int iDouble = 0;
    unsigned int iString = 0;

    fprintf(fwt, "#Zacatek skriptu\n");
    fprintf(fwt, "%s\n", kindName.c_str());
    for(loop1 = 0; loop1 < data.size(); loop1++)
    {
        fprintf(fwt, "%s", data[loop1].name.c_str());
        switch(data[loop1].kind)
        {
            case INTEGER :
                fprintf(fwt, "\n%d\n\n", intData[iInteger++]);
                break;
            case STRING :
                fprintf(fwt, "\n%s\n\n", stringData[iString++].c_str());
                break;
            case DOUBLE :
                fprintf(fwt, "\n%f\n\n", doubleData[iDouble++]);
                break;
            case TILE_LINK :
                fprintf(fwt, "\n");
                fprintf(fwt, "%d tiles\n", tileData[iTileLink].size());
                for(loop2 = 0; loop2 < tileData[iTileLink].size(); loop2++)
                {
                    fprintf(fwt, "%d %d\n", tileData[iTileLink][loop2].x, tileData[iTileLink][loop2].y);
                }
                fprintf(fwt, "\n");
                iTileLink++;
                break;
        }
    }
    fprintf(fwt, "#Konec skriptu\n\n");
    OLD SAVE TEXT SCRIPT */
}

void C_Script::Load(FILE *fr)
{
    unsigned int loop1, loop2;
    unsigned int tempSize, tempSize2;
    S_ScriptData temp;
    string tempString;
    int tempInt;
    double tempDouble;
    vector<S_MAPPOS> tempVecTile;
    S_MAPPOS tempTile;

    FileMan::Inst()->LoadInteger(tempInt, fr);
    iGroup = tempInt;
    FileMan::Inst()->LoadString(kindName, fr);
    FileMan::Inst()->LoadString(name, fr);

    int signedInt;
    FileMan::Inst()->LoadInteger(signedInt, fr);
    tempSize = signedInt;
    for(loop1 = 0; loop1 < tempSize; loop1++)
    {
        data.push_back(temp);
        fread(&data[loop1].kind, sizeof(E_SCRIPTDATAKIND), 1, fr);
        FileMan::Inst()->LoadString(data[loop1].name, fr);
        FileMan::Inst()->LoadString(data[loop1].help, fr);
        FileMan::Inst()->LoadInteger(data[loop1].value[0], fr);
        FileMan::Inst()->LoadInteger(data[loop1].value[1], fr);
        FileMan::Inst()->LoadInteger(data[loop1].value[2], fr);
    }

    FileMan::Inst()->LoadInteger(signedInt, fr);
    tempSize = signedInt;
    for(loop1 = 0; loop1 < tempSize; loop1++)
    {
        stringData.push_back(tempString);
        FileMan::Inst()->LoadString(stringData[loop1], fr);
    }
    FileMan::Inst()->LoadInteger(signedInt, fr);
    tempSize = signedInt;
    for(loop1 = 0; loop1 < tempSize; loop1++)
    {
        intData.push_back(tempInt);
        FileMan::Inst()->LoadInteger(intData[loop1], fr);
    }
    FileMan::Inst()->LoadInteger(signedInt, fr);
    tempSize = signedInt;
    for(loop1 = 0; loop1 < tempSize; loop1++)
    {
        doubleData.push_back(tempDouble);
        FileMan::Inst()->LoadDouble(doubleData[loop1], fr);
    }

    FileMan::Inst()->LoadInteger(signedInt, fr);
    tempSize = signedInt;
    for(loop1 = 0; loop1 < tempSize; loop1++)
    {
        tileData.push_back(tempVecTile);
        FileMan::Inst()->LoadInteger(signedInt, fr);
        tempSize2 = signedInt;
        for(loop2 = 0; loop2 < tempSize2; loop2++)
        {
            tileData[loop1].push_back(tempTile);
            FileMan::Inst()->LoadInteger(tileData[loop1][loop2].x, fr);
            FileMan::Inst()->LoadInteger(tileData[loop1][loop2].y, fr);
        }
    }
}

void C_Script::Show()
{
    vector<S_MAPPOS> showTile;
    for(unsigned int loop1 = 0; loop1 < tileData.size(); loop1++)
        showTile.insert(showTile.end(), tileData[loop1].begin(), tileData[loop1].end());

    C_LevelEditor::Inst()->MarkedTile(showTile);
}

void C_Script::TranslateTiles(int dX, int dY)
{
    C_Map * pMap = C_LevelEditor::Inst()->GetMap();

    unsigned int loop1, loop2;
    for(loop1 = 0; loop1 < tileData.size(); loop1++)
    {
        for(loop2 = 0; loop2 < tileData[loop1].size(); loop2++)
        {
            tileData[loop1][loop2].x += dX;
            tileData[loop1][loop2].y += dY;
            if(tileData[loop1][loop2].x < 0 || tileData[loop1][loop2].x > (signed) pMap->GetMapWidth() - 1 ||
               tileData[loop1][loop2].y < 0 || tileData[loop1][loop2].y > (signed) pMap->GetMapHeight()- 1)
            {
               tileData[loop1].erase(tileData[loop1].begin() + loop2, tileData[loop1].begin() + loop2 + 1);
               loop2--;
            }
        }
    }
}

C_Script::~C_Script()
{
    data.clear();
    intData.clear();
    doubleData.clear();
    stringData.clear();
}
