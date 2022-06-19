//redimentionne une image

uint16_t* resize(uint16_t* i_img, int originalW, int originalH, float f)
{
    int newW = originalW/f;
    int newH = originalH/f;

    Serial.println("resize: " + String(originalW) + " " + String(newW));

    int proportion = (f>int(f))?(f+1):(f); // use the best proportion for better quality
    proportion=2;

    Serial.println("proportion: " + String(proportion));

    uint16_t* o_img = (uint16_t*)ps_malloc(newW*newH*sizeof(uint16_t));

    for(int x = 0; x < newW; x++)
    {
        for(int y = 0; y < newH; y++)
        {
            int moy[3]={0,0,0};
            int iterator = 0;
            
            int bx = x*f-proportion/2;
            int by = y*f-proportion/2;

            if(bx<0)
                bx = 0;
            if(by<0)
                by = 0;

            for(int mx = bx; mx < bx + proportion; mx++)
            {
                for(int my = by; my < by + proportion; my++)
                {
                    if(mx < originalW && my < originalH)
                    {
                        uint16_t color = i_img[mx+my*originalW];

                        byte r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
                        byte g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
                        byte b = (((color & 0x1F) * 527) + 23) >> 6;

                        moy[0]+=r;
                        moy[1]+=g;
                        moy[2]+=b;

                        iterator++;
                    }
                }
            }

            o_img[x+y*newW] = tft_root.color565(moy[0]/iterator,moy[1]/iterator,moy[2]/iterator);
        }
    }
    
    return o_img;
}