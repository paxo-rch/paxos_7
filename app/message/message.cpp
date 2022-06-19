#ifndef APP_MESSAGE_CPP
#define APP_MESSAGE_CPP

#define SIZE_SPACE_MESSAGE 40

#include "message.hpp"

bool emojExist(String str)
{
    if(str.length()!=8)
        return 0;
    String emojFile = readFile("/paxo/apps/messages/emoj/emoj.txt");
    return emojFile.indexOf(str) != -1;
}

void message::launch()
{
    home();
}

void message::home()
{
    Window window("contacts");

    Box* contactBox = new Box(35, 130, 250, 324);
    window.addChild(contactBox);

    contactBox->setBorderSize(0);
    contactBox->verticalSlide=1;

    Switch* switchTest = new Switch(100, 400, 0);
    //window.addChild(switchTest);

    while (true)
    {
        File root = SD.open("/paxo/apps/messages/chat");
        File file = root.openNextFile();

        int conversation_number = 0;

        for (int i = 0;file;i++)
        {
            if (!file.isDirectory())
            {
                String str = readFile(file.name());
                String name = "";
                for(int j = 0;; j++)
                {
                    if(str[j] != ':')
                        name += str[j];
                    else
                    {
                        String contact = getContact(name);

                        if(contact != "-1")
                        {
                            Button* openConversation = new Button(0, i*SIZE_SPACE_MESSAGE, 250, SIZE_SPACE_MESSAGE-5, contact);
                            openConversation->setRadius(0);
                            openConversation->setBorderSize(0);
                            openConversation->appCallback=this;
                            openConversation->onreleased=open;
                            openConversation->dataCallback=(void *)i;
                            openConversation->alignement=LEFT_ALIGNMENT;
                            contactBox->addChild(openConversation);
                        }else
                        {
                            Button* openConversation = new Button(0, i*SIZE_SPACE_MESSAGE, 250, SIZE_SPACE_MESSAGE-5, name);
                            openConversation->setRadius(0);
                            openConversation->setBorderSize(0);
                            openConversation->appCallback=this;
                            openConversation->onreleased=open;
                            openConversation->dataCallback=(void *)i;
                            openConversation->alignement=LEFT_ALIGNMENT;
                            contactBox->addChild(openConversation);
                        }

                        conversation_number++;
                        break;
                    }
                }
            }
            file = root.openNextFile();
        }

        switchTest->onreleased=changeTheme;
        switchTest->appCallback=this;


        window.drawAll();

        while (true)
        {
            window.updateAll();
        }

        window.removechildren();
    }
}

void message::conversation(int index)
{
    Window window("conversasion");
    window.setBackgroundColor(0xffff);
    
    tft.fillScreen(0xffff);

    Box* bulls= new Box(0,0,320,285);
    bulls->setBorderSize(0);

    while(1)
    {
        bool stopParsing = false;

        String str = "";
        String phoneNumber = "";

        File root = SD.open("/paxo/apps/messages/chat");
        File file = root.openNextFile();

        for (int i = 0;file;i++)
        {
            if (!file.isDirectory() && i == index)
            {
                String brute_str = readFile(file.name());
                phoneNumber = brute_str.substring(0, brute_str.indexOf(':'));
                str = brute_str.substring(brute_str.indexOf(':') + 1, brute_str.length());
                break;
            }
            file = root.openNextFile();
        }

        window.setTitle(phoneNumber);

        String conversation = "";
        String buffer_="";

        int pixel_counter = 322;

        str+=" ";

        int nb_bull = 0;

        for (int i = str.length(); i > 0 && nb_bull < 20; i--) // parse the file
        {
            
            buffer_ = str[i] + buffer_;
            if(buffer_.indexOf("\n~MB~") != -1)
            { 
                nb_bull++;
                buffer_ = buffer_.substring(5, buffer_.indexOf("~ME~"));
                Label* bull = new Label(130, 0, 180, 40);

                bull->setText(buffer_);
                bull->setTextColor(0x29A6);
                bull->setRadius(12);

                bull->setBorderSize(2);
                bull->setBorderColor(0xE73C);
                
                int sizeOfBull = bull->getHeightOfText();
                pixel_counter-=sizeOfBull+5;

                bull->setY(pixel_counter + 2);

                bull->setHeight(sizeOfBull);

                bulls->addChild(bull);
                bull->setBackgroundColor(0xffff);
            }
            if(buffer_.indexOf("~OB~") != -1)
            {
                nb_bull++;
                buffer_ = buffer_.substring(4, buffer_.indexOf("~OE~"));

                if(emojExist(buffer_))
                {
                    pixel_counter -= 55;
                    Image* emoj = new Image("/paxo/apps/messages/emoj/"+buffer_+".bmp", 10, pixel_counter);
                    emoj->load();
                    //emoj->load();
                    bulls->addChild(emoj);
                }else
                {
                    Label* bull = new Label(10, 0, 180, 40);
                    bull->setText(buffer_);
                    bull->setTextColor(0x29A6);
                    bull->setRadius(12);

                    bull->setBorderSize(2);
                    bull->setBorderColor(0xE73C);
                    
                    int sizeOfBull = bull->getHeightOfText();
                    pixel_counter-=sizeOfBull+5;

                    bull->setY(pixel_counter + 2);

                    bull->setHeight(sizeOfBull);
                    
                    bulls->addChild(bull);
                    bull->setBackgroundColor(0xffff);
                }
            }
        }

        Keyboard* key = new Keyboard();

        Label* toSend = new Label(0, 300, 320, 50);
        toSend->setTextColor(0x29A6);
        toSend->setBackgroundColor(0xFFFF);

        window.addChild(bulls);
        window.addChild(key);
        window.addChild(toSend);
        
        toSend->draw();

        window.drawAll();

        bulls->verticalSlide=true;

        while (1)
        {
            if(home_button.pressed())
            {
                window.removechildren();
                return;
            }

            window.updateAll();
            char chr = key->isTouch();
            if(chr!=0)
            {
                if(chr!='\n')
                {
                    uint16_t toSendSize = toSend->getHeight();

                    if(chr==KEY_ERASE)
                        toSend->setText(toSend->getText().substring(0, toSend->getText().length()-1));
                    else
                        toSend->setText(toSend->getText() + String(chr));

                    toSend->setHeight(toSend->getHeightOfText());

                    if(toSend->getHeight()>=toSendSize)
                        toSend->drawAll();
                    else
                    {
                        bulls->drawAll();
                        toSend->drawAll();
                    }
                }
                else
                {
                    phoneNumber = "+33" + phoneNumber.substring(1, phoneNumber.length());
                    
                    SIM.println("AT+CMGF=1");
                    delay(100);
                    SIM.println("AT+CMGS=\"" + phoneNumber + "\"\r");
                    delay(100);
                    SIM.print(toSend->getText());
                    SIM.print('\r');
                    delay(100);
                    SIM.write(char(26));
                    SIM.println('\r');
                    delay(100);
                    appendFile(file.name(), "\n~MB~"+toSend->getText()+"~ME~");
                    toSend->setText("");
                    break;
                }
            }
        }

        window.removechildren();
    }
}

String message::getContact(String number)
{
    String str = readFile("/paxo/apps/messages/contacts.txt");
    int index = str.indexOf(number);
    if(index == -1) 
        return "-1";
    String contact = "";
    for(; str[index-1] != ':'; index++);
    for(; str[index] != '\n'; index++)
        contact+=str[index];
    return contact;
}

#endif