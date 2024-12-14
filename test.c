#define unsigned char uint8_t
void GrayToPseColor(uint8_t converMethod,uint8_t grayValue, uint8_t *colorR,uint8_t *colorG,uint8_t *colorB)  //灰度-伪彩色变换
{
	switch(converMethod)
	{
		case GCM_Pseudo1:
			*colorR=abs(0-grayValue);
			*colorG=abs(127-grayValue);
			*colorB=abs(255-grayValue);		
			break;
			
		case GCM_Pseudo2:	
			if( (grayValue>=0) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=0;
				*colorB=round(grayValue/64*255);
			}
			else if( (grayValue>=64) && (grayValue<=127) )  
			{
				*colorR=0;
				*colorG=round((grayValue-64)/64*255);
				*colorB=round((127-grayValue)/64*255);
			}
			else if( (grayValue>=128) && (grayValue<=191) )  
			{
				*colorR=round((grayValue-128)/64*255);
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=192) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=round((255-grayValue)/64*255);
				*colorB=0;
			}		
			break;

		case GCM_Metal1:	
			if( (grayValue>=0) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=0;
				*colorB=round(grayValue/64*255);
			}
			else if( (grayValue>=64) && (grayValue<=95) )  
			{
				*colorR=round((grayValue-63)/32*127);
				*colorG=round((grayValue-63)/32*127);
				*colorB=255;
			}
			else if( (grayValue>=96) && (grayValue<=127) )  
			{
				*colorR=round((grayValue-95)/32*127)+128;
				*colorG=round((grayValue-95)/32*127)+128;
				*colorB=round((127-grayValue)/32*255);
			}
			else if( (grayValue>=128) && (grayValue<=191) )  
			{
				*colorR=255;
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=192) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=255;
				*colorB=round((grayValue-192)/64*255);
			}
			break;
			
		case GCM_Metal2:	
			*colorR=0;*colorG=0;*colorB=0;
			if( (grayValue>=0) && (grayValue<=16) )  
			{
				*colorR=0;
			}
			else if( (grayValue>=17) && (grayValue<=140) )  
			{
				*colorR=round((grayValue-16)/(140-16)*255);
			}
			else if( (grayValue>=141) && (grayValue<=255) )  
			{
				*colorR=255;
			}

			if( (grayValue>=0) && (grayValue<=101) )  
			{
				*colorG=0;
			}
			else if( (grayValue>=102) && (grayValue<=218) )  
			{
				*colorG=round((grayValue-101)/(218-101)*255);
			}
			else if( (grayValue>=219) && (grayValue<=255) )  
			{
				*colorG=255;
			}

			if( (grayValue>=0) && (grayValue<=91) )  
			{
				*colorB=28+round((grayValue-0)/(91-0)*100);
			}
			else if( (grayValue>=92) && (grayValue<=120) )  
			{
				*colorB=round((120-grayValue)/(120-91)*128);
			}
			else if( (grayValue>=129) && (grayValue<=214) )  
			{
				*colorB=0;
			}
			else if( (grayValue>=215) && (grayValue<=255) )  
			{
				*colorB=round((grayValue-214)/(255-214)*255);
			}
			break;		
			
		case GCM_Rainbow1:
			if( (grayValue>=0) && (grayValue<=31) )  
			{
				*colorR=0;
				*colorG=0;
				*colorB=round(grayValue/32*255);
			}
			else if( (grayValue>=32) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=round((grayValue-32)/32*255);
				*colorB=255;
			}
			else if( (grayValue>=64) && (grayValue<=95) )  
			{
				*colorR=0;
				*colorG=255;
				*colorB=round((95-grayValue)/32*255);
			}
			else if( (grayValue>=96) && (grayValue<=127) )  
			{
				*colorR=round((grayValue-96)/32*255);
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=128) && (grayValue<=191) )  
			{
				*colorR=255;
				*colorG=round((191-grayValue)/64*255);
				*colorB=0;
			}
			else if( (grayValue>=192) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=round((grayValue-192)/64*255);//0
				*colorB=round((grayValue-192)/64*255);
			}		
			break;
			
		case GCM_Rainbow2:
			if( (grayValue>=0) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=round((grayValue-0)/64*255);
				*colorB=255;
			}
			else if( (grayValue>=64) && (grayValue<=95) )  
			{
				*colorR=0;
				*colorG=255;
				*colorB=round((95-grayValue)/32*255);
			}
			else if( (grayValue>=96) && (grayValue<=127) )  
			{
				*colorR=round((grayValue-96)/32*255);
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=128) && (grayValue<=191) )  
			{
				*colorR=255;
				*colorG=round((191-grayValue)/64*255);
				*colorB=0;
			}
			else if( (grayValue>=192) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=round((grayValue-192)/64*255);
				*colorB=round((grayValue-192)/64*255);
			}
			break;
			
		case GCM_Rainbow3:
			if( (grayValue>=0) && (grayValue<=51) )  
			{
				*colorR=0;
				*colorG=grayValue*5;
				*colorB=255;
			}
			else if( (grayValue>=52) && (grayValue<=102) )  
			{
				*colorR=0;
				*colorG=255;
				*colorB=255-(grayValue-51)*5;
			}
			else if( (grayValue>=103) && (grayValue<=153) )  
			{
				*colorR=(grayValue-102)*5;
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=154) && (grayValue<=204) )  
			{
				*colorR=255;
				*colorG=round(255-128*(grayValue-153)/51);
				*colorB=0;
			}
			else if( (grayValue>=205) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=round(127-127*(grayValue-204)/51);
				*colorB=0;
			}
			break;
		
		case GCM_Zhou:
			if( (grayValue>=0) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=round((64-grayValue)/64*255);
				*colorB=255;
			}
			else if( (grayValue>=64) && (grayValue<=127) )  
			{
				*colorR=0;
				*colorG=round((grayValue-64)/64*255);
				*colorB=round((127-grayValue)/64*255);
			}
			else if( (grayValue>=128) && (grayValue<=191) )  
			{
				*colorR=round((grayValue-128)/64*255);
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=192) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=round((255-grayValue)/64*255);
				*colorB=0;
			}		
			break;
			
		case GCM_Ning:
			if ((grayValue>=0) && (grayValue<=63))  
			{
				*colorR=0;
				*colorG=254-4*grayValue;
				*colorB=255;
			}
			else if ((grayValue>=64) && (grayValue<=127))  
			{
				*colorR=0;
				*colorG=4*grayValue-254;
				*colorB=510-4*grayValue;
			}
			else if ((grayValue>=128) && (grayValue<=191))  
			{
				*colorR=4*grayValue-510;
				*colorG=255;
				*colorB=0;
			}
			else if ((grayValue>=192) && (grayValue<=255))  
			{
				*colorR=255;
				*colorG=1022-4*grayValue;
				*colorB=0;
			}
			break;
			
		case GCM_Gray:	
			*colorR=grayValue;
			*colorG=grayValue;
			*colorB=grayValue;
			break;
			
		default:
			break;
	}
}