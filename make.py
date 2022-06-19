import os
from os import listdir
from os.path import isfile, join

# scan apps and include them

FichList = [ f for f in os.listdir('./app') if( os.path.isfile(os.path.join('./app',f)) == False) ]

# make the apps.hpp file

file_app = """
#ifndef APPS_HPP
#define APPS_HPP

#include "../widgets/gauss.hpp"
#include "../system/files/files.hpp"

class App
{
    public:
    virtual void launch() = 0;
    virtual String getName() = 0;
    virtual String getLogo() = 0;
    Window* window = nullptr;
};

bool watchDogCall()
{
    return true;
}

#define while(condition) while(watchDogCall() && condition)
#define appEvent static void

"""

for f in FichList:
    file_app += "#include \""+f+"/"+f+".hpp\"\n"

file_app += "#define NB_APPS " + str(len(FichList))
file_app += "\nApp *application[NB_APPS] = {\n"

for f in FichList:
    file_app += "nullptr,"

file_app = file_app[0:len(file_app)-1]

file_app += "\n};\n"

file_app += """
void allocateApp(int index)
{
    switch (index)
    {
"""

i=0

for f in FichList:
    file_app += "       case "+str(i)+": application["+str(i)+"] = new "+f+"; break;\n"
    i+=1

file_app += """
    }   
}

"""

#############

file_app += """
void destroyApp(int index)
{
    switch (index)
    {
"""

i=0

for f in FichList:
    file_app += "       case "+str(i)+": delete application["+str(i)+"]; break;\n"
    i+=1

file_app += """
    }   
}

"""

########

file_app += """
void runApp(int index)
{
    allocateApp(index);
    if(application[index]!=nullptr) application[index]->launch();
    else Serial.println("invalid app index:" + String(index));
    destroyApp(int(index));
    //vTaskDelete(0);
}

#endif /* APPS_HPP */
"""

fichier = open('./app/apps.hpp', "w")
fichier.write(file_app)
fichier.close()

os.system('arduino --upload PAXOS_7.ino')