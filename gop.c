static int set_gop(unsigned char *buf_addr, INFOHEADER *pBmpInfo, int w_panel, int h_panel)
{
    unsigned int bk10reg;
    unsigned int width, height;
    unsigned int clrfmt;
    unsigned int reglength;
    unsigned int h_ratio, v_ratio;
    unsigned int gop_word;

    gop_word = 32;

    width = pBmpInfo->width;
    height = pBmpInfo->height;
    clrfmt = pBmpInfo->clrfmt;

    REG_ADDR(0x1FFE) =  (REG_ADDR(0x1FFE) & (~0x00FF)); /* switch to bank 0 */

    REG_ADDR(0x2F00) = 0x0000;
    REG_ADDR(0x2F0C) = 0xF000; /* enable SC_OP */

    REG_ADDR(0x1FFE) = 0x200;  /* Switch to GOP0*/

    REG_ADDR(0x1F00) = 0x510C; /* Reset GOP */

    REG_ADDR(0x1F02) = (REG_ADDR(0x1F02) & (~0x07))|0x02; /* Set GOP dst to OP */
#if 0
#if defined(PRJ_X9)
    REG_ADDR(0x1F1E) = panel_hstart + 0x7F; /* Pipeline offset */
#else
    REG_ADDR(0x1F1E) = panel_hstart + 0xAE; /* Pipeline offset */
#endif
#endif
    REG_ADDR(0x1F1C) = 0x01e1;
    REG_ADDR(0x1F1E) = 0x0143;
    REG_ADDR(0x1F60) = width/2; /* Stretch window H */
    REG_ADDR(0x1F62) = height; /* Stretch window V */

    REG_ADDR(0x1F00) = (REG_ADDR(0x1F00)&(~0x3000)); /* Clear HV mirror */
    if (Hmirror)
    {
        REG_ADDR(0x1F00) = REG_ADDR(0x1F00)|0x1000; /* enable H mirror */
    }
    if (Vmirror)
    {
        REG_ADDR(0x1F00) = REG_ADDR(0x1F00)|0x2000; /* enable V mirror */
    }

    if (full_screen) /* Full Screen */
    {
        /* Enlarge */
        h_ratio = (width*4096)/w_panel; /* h-ratio */
        v_ratio = (height*4096)/h_panel; /* V-ratio */
        pos_x = 0;
        pos_y = 0;
    }
    else
    {
        h_ratio = 0x1000;
        v_ratio = 0x1000;

        if (full_scale)
        {
            h_ratio = (width*4096)/w_panel;
            v_ratio = h_ratio;
            pos_x = 0;
        }
        else
        {
            if (double_size)
            {
                if ((w_panel >= (width*2))&&(h_panel >= (height*2)))
                {
                    h_ratio = 0x800;
                    v_ratio = 0x800;
                }
            }
        }
    }
    REG_ADDR(0x1F6A) = h_ratio; /* h-ratio */
    REG_ADDR(0x1F6C) = v_ratio; /* V-ratio */

    /* Position */
    if (pos_x < 0)    //pos_x is center
    {
        pos_x = (w_panel - width)/2;
    }

    if (pos_y < 0)    //pos_y is center
    {
        pos_y = (h_panel - height)/2;
    }
    else
    {
        /* check Vmirror */
        if(Vmirror)
        {
            pos_y = (h_panel - (height*w_panel/width) - pos_y);
        }
    }

    REG_ADDR(0x1F64) = pos_x; /* X */
    REG_ADDR(0x1F68) = pos_y; /* Y */

    REG_ADDR(0x1FFE) =  REG_ADDR(0x1FFE) | 0x0001; /* switch to bank 1 */

//#if defined(PRJ_X10P) || defined(SN)
    /* Buffer address access */
    //buf_addr = (unsigned char *)((unsigned long)LOGO_MIU_PHY_ADDR);
    REG_ADDR(0x1F02) = (((unsigned long)buf_addr)/gop_word)&0xFFFF;
    REG_ADDR(0x1F04) = ((((unsigned long)buf_addr)/gop_word))>>16;

    REG_ADDR(0x1F08) = 0x0000; /* set GWin start */
    REG_ADDR(0x1F0A) = width/8; /* set GWin end */

    REG_ADDR(0x1F0C) = 0x0000; /* set GWin Height */
    REG_ADDR(0x1F10) = height; /* set GWin Height */

    REG_ADDR(0x1F12) = width/8; /* set Gwin Pitch */

    REG_ADDR(0x1F14) = 0x00FF; /* Alpha Value */

    reglength = width*height*(pBmpInfo->bits)/8; /* WxHxBPP */

    //REG_ADDR(0x1F18) = ((reglength/gop_word) - (width/8))&0xFFFF; /* WxHxBPP/16 - W/4 */
    //REG_ADDR(0x1F1A) = (((reglength/gop_word) - (width/8))>>16)&0xFFFF;
    //REG_ADDR(0x1F20) = (reglength/gop_word)&0xFFFF; /* WxHxBPP/16 */
    //REG_ADDR(0x1F22) = ((reglength/gop_word)>>16)&0xFFFF;
    REG_ADDR(0x1F18) = 0x0000;
    REG_ADDR(0x1F1A) = 0x0000;
    REG_ADDR(0x1F20) = 0x0000;
    REG_ADDR(0x1F22) = 0x0000;

    REG_ADDR(0x1F40) = 0x00A0;

    REG_ADDR(0x1FFC) = 0x0000; /* set mux to GOP0 */
    REG_ADDR(0x0B80) = (REG_ADDR(0x0B80) & (~0x00FF))|0x00;  /* Set mixer to ODCLK */

    bk10reg = REG_ADDR(0x2F00);
    REG_ADDR(0x2F00) = 0x10;
    REG_ADDR(0x2F46) = 0xA0;
    REG_ADDR(0x2F00) = bk10reg;

    REG_ADDR(0x1F00) = 0x3F01 + (clrfmt<<4);// & 0x00F0); /* set color format & enable GWIN */
    REG_ADDR(0x1FFE) =  REG_ADDR(0x1FFE) & 0xFFFE; /* switch back to bank 0 */

    /* for Sharp panel flicker issue */
    bk10reg = REG_ADDR(0x2F00);
    REG_ADDR(0x2F00) = 0x19;
    REG_ADDR(0x2F20) = 0x80;
    REG_ADDR(0x2F00) = bk10reg;

    //TODO: only for A3 A5 A5p Agate , GOP0 MIU SEL = 0x12F6 BIT2 3 4 5 -> GOP0 1 2 3

    if (logo_phy_addr>= LOGO_MIU0_MEM_START && logo_phy_addr < LOGO_MIU1_MEM_START)
    {
        REG_ADDR(GOP0MIUSEL) &= ~BIT2; //gop0 for miu0
    }
    else
    {
        REG_ADDR(GOP0MIUSEL) |= BIT2; //gop0 for miu1
    }

    return 0;
}

void mute_logo(void)
{
    REG_ADDR(0x1FFE) =  (REG_ADDR(0x1FFE) & (~0x0FFF))|0x201; /* switch to bank 1 */

    REG_ADDR(0x1F00) = REG_ADDR(0x1F00) & (~0x0001); /* disable GWIN */

    REG_ADDR(0x1FFE) =  (REG_ADDR(0x1FFE) & (~0x0FFF)); /* switch to bank 0 */
}

static int __init show_logo_init(void)
{
    int waitloop = 0;

    use_logo = logo_file;

    logo_threadfinish = false ;

    // Backup Gop miu sel
    gopMiuSelBackup = REG_ADDR(GOP0MIUSEL);

    // Run kthread
    printk(KERN_INFO " 3 - logo_thread() run\n");
    ts = kthread_run(logo_thread, NULL, "kthread");
    printk("wait - 0  finish\n");

    while(!logo_threadfinish)
    #if 1
    {

        msleep(50);
        //printk("wait - 1\n");

        // avoid dead loop
        if( waitloop++ > 10 )
            break ;
    }
    #endif
    printk("wait - 1  finish\n");

    msleep(display_time);
    // Diable GOP0 GWin
    REG_ADDR(0x1FFE) =  REG_ADDR(0x1FFE) | 0x0001; /* switch to bank 1 */
    REG_ADDR(0x1F00) = REG_ADDR(0x1F00) & (~0x0001); /* disable GWIN */
    REG_ADDR(0x1FFE) =  REG_ADDR(0x1FFE) & 0xFFFE; /* switch to bank 0 */

    // Recover Gop miu sel
    REG_ADDR(GOP0MIUSEL) = gopMiuSelBackup;


    if(logo_buf)
        dma_free_coherent(NULL,tLogoInfo.width*tLogoInfo.height*tLogoInfo.bits,logo_buf,logo_phy_addr);
    printk("wait - 2 finish\n");

    return 0;
}