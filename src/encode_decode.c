#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// count,sort,encode(make new encoded string)

struct node{
	int freq;
	char denote;
	struct node *left;
	struct node *right;
}**add_arr;

struct char_arr{
    char ch;
    int ch_freq;
};
struct char_arr *ch_arr;

int len=0;
int s_len = 0;
int ch_code_len=0;
int output_len = 0;
int freq_arr[96] = {0};
char ch_codes[96][8] = {{'\0'}};   //review not to be made global

void sort(int i)
{
    for(;i<len-1;i++)
    {
        int min = i;
        for(int j=i+1;j<len;j++)
        {
            if(ch_arr[j].ch_freq<ch_arr[min].ch_freq)
            {
                min = j;
            }
        }
        if(min!=i)
        {
            int temp_freq = ch_arr[min].ch_freq;
            ch_arr[min].ch_freq = ch_arr[i].ch_freq;
            ch_arr[i].ch_freq = temp_freq;
            
            char temp_ch = ch_arr[min].ch;
            ch_arr[min].ch = ch_arr[i].ch;
            ch_arr[i].ch = temp_ch;

            struct node *temp3_addr = add_arr[min];
			add_arr[min] = add_arr[i];
			add_arr[i] = temp3_addr;
        }
    }
}

struct node* create_node(int n,struct node *l,struct node *r)
{
	static char denotee = 'A';
	struct node *new;
	new = (struct node*)malloc(sizeof(struct node));
	new->freq = ch_arr[n].ch_freq;// freq;
	if((l!=NULL)&&(r!=NULL))
	{
		new->denote = denotee++;
	}
	else
	{
		new->denote = ch_arr[n].ch;//den;
	}
	new->left = l;
	new->right = r;
	//printf("%d %c %p %p %p\n",new->freq,new->denote,new,new->left,new->right);
	return new;
}

void count_freq(char* ch)
{
    s_len = (int)strlen(ch);
    for(int i;i<strlen(ch);i++)
    {
        int c = ch[i]-32;
        freq_arr[c]++;
        if(freq_arr[c]==1)
        {
            len++;
        }
    }//printf("\nvalue of len is %d\n",len);
}

void assign_freq()
{
    ch_arr = (struct char_arr*)calloc(len,sizeof(struct char_arr));
    int count=0;
    for(int i = 0;i < 96;i++)
    {
        if(freq_arr[i]>0)
        {
            ch_arr[count].ch = i+32;
            ch_arr[count].ch_freq = freq_arr[i];
            count++;
        }
    }
}



void generate_code(struct node *root,const char *code)
{
    //printf("\nhere in generate");
	if(root==NULL)
	{
		return;
	}
	char str[10] = "";
	strcpy(str,code);
	strcat(str,"0");
	generate_code(root->left,str);
	str[strlen(str)-1] = '1';
	generate_code(root->right,str);
	if(root->left==NULL)
    {
        char ch = root->denote;
        ch_code_len+=(int)strlen(code);
        output_len+=(int)(strlen(code)*(root->freq));
        strcpy(&ch_codes[ch-32][8],code);
		//printf("%s\t%c\t%s\t%d\t%d\n",code,ch,&ch_codes[ch-32][8],output_len,ch_code_len);
	}
}


void huffman()
{
    //printf("here in huffman %d",len);
    add_arr = (struct node**)calloc(len,sizeof(struct node*));

    int i=0;
	//sort(i);
	for( ; i < len-1 ; i++)
	{
		sort(i);
		if(add_arr[i]==NULL)
		{
			add_arr[i] = create_node(i,NULL,NULL);
		}
		if(add_arr[i+1]==NULL)
		{
			add_arr[i+1] = create_node(i+1,NULL,NULL);
		}
		ch_arr[i+1].ch_freq += ch_arr[i].ch_freq;
		add_arr[i+1] = create_node(i+1,add_arr[i],add_arr[i+1]);
	}
    char *code = "";
    generate_code(add_arr[i],code);
    //printf("back in huffman\n");
}

char* encode(char* s)
{
    count_freq(s);
    assign_freq();
    
    //memset(ch_codes,'2',96*8*sizeof(char));
    
    huffman();
    //missing work
    //first append character codes in string,
    //append the encoding
    int final_str_len = output_len+ch_code_len+200-len;
    //printf("%d %d %d",strlen(s),len,(output_len);
    //printf("%s %s",&ch_codes['-'-32][8],&ch_codes['h'-32][8]);
    char *final_str = (char*)malloc((final_str_len)*sizeof(char));
    //char *final_str[] = "\0";
    final_str[0] = '\0';
    //printf("start\n");
    for(int i=0;i< 96;i++)   //final str main ch_code strcat
    {
       if(strcmp(&ch_codes[i][8],"")==0)
       {
           strcat(final_str,"2");
       }
       else
       {
           strcat(final_str,&ch_codes[i][8]);  
       }
       strcat(final_str,"\n");
    }
    //final_str[(int)strlen(final_str)] = '\0';
    for(int i=0;i<(int)strlen(s);i++) //final str main out str banake strcat
    {
        strcat(final_str,&ch_codes[s[i]-32][8]); 
        //strcat(&final_str[(int)strlen(final_str)-1],&ch_codes[s[i]-32][8]); 
    }
    //printf("%s\n",final_str);
    //printf("%s",&ch_codes[0][8]);
    //printf("\n%s",&final_str[3]);
    return final_str;
}
/*
void search(char *input_str,char *check,char chd_codes[96][8])
{
    for(int i=0;i<96;i++)
    {
        if(strcmp(&chd_codes[i][8],check)==0)
        {
            printf("%c",(char)(i+32));
            input_str+=(int)strlen(check);
        }
    }
}*/

char* decode(char* input_str)
{
    int input_len = (int)strlen(input_str);
    char chd_codes[96][8];
    int max=0;
    //printf("\nhere in decode %d\n",len);
    //take out ch_codes from the input_string
    for(int i=0;i<96;i++)
    {
        sscanf(input_str,"%s",&chd_codes[i][8]);
        //char *cmp = "2";
        if(strcmp(&chd_codes[i][8],"2")!=0)
        {
            //printf("%s ",&chd_codes[i][8]);
            input_str+=(int)(strlen(&chd_codes[i][8])+1);
            if(max<(int)strlen(&chd_codes[i][8]))
            {
                max = (int)strlen(&chd_codes[i][8]);
            }
        }
        else
        {
            input_str+=2;
        }
    }
    printf("\nEncoded String is:%s\n",input_str);
    int a= output_len, b = (s_len*8);
    printf("Length of String Encoded with huffman algorithm : %d\n",a);
    printf("Length of String Encoded without huffman algorithm : %d\n",b);
    float eff = (b-a)*100/b;
    printf("Effciency : %f\n",eff);
    //int updated_len = (int)strlen(input_str);
    /*for(int i=0;i<updated_len;i++)
    {
       /* for(int j=0;j<max;j++)//2nd for loop length is atmost max ch_code length
         {
            
         }*/
         //check()    
    //} 
    //char *output_str = "\0";
    char *output_str = (char*)malloc(256*sizeof(char));
    //output_str = "0";
    while((input_str[0]!='\0')/*||(input_str[0]!=NULL)*/)
    {
        
        //printf("\ninside %s",input_str);
        for(int i = 0;i < 96; i++)
        {
            if(strcmp(&chd_codes[i][8],"2")!=0)
            {
                int len_match = (int)strlen(&chd_codes[i][8]);
                char form_spec[4];
                strcpy(form_spec,"% s");
                form_spec[1] = (char)(len_match+48);
                char check[10];
                
                sscanf(input_str,form_spec,check);
                //printf("\n%s %s %s",&chd_codes[i][8],form_spec,check);
                //const char *cc;
                //strcpy(cc,check);
                if(strcmp(check,&chd_codes[i][8])==0)
                {
                    char *c;
                    sprintf(c,"%c",(char)(i+32));
                    //c[0] = (char)(i+32);
                    
                    //printf("\n%s",c);
                    strcat(output_str,c);
                    input_str+=len_match;
                }
            }
        }
        /*for(int i=1;i<=max;i++) //remove it
        {
            char form_spec[4];
            char *check;
            strcpy(form_spec,"% s");
            form_spec[1] = (char)(i+48);
            printf("inside for");
            sscanf(input_str,form_spec,check);
            search(input_str,check,chd_codes);
        }*/
    }
    return &output_str[3];
}



void main()
{
    printf("Please enter text to be Coded and then Decoded using Huffman Algorithm as base : ");
    char message[255];// = "Hi Nisha~nt,how are you?How is your son.Fine.!!. I am fine ,Thank You";
    scanf("%[^\n]%*c", &message);
    char* encoded = encode(message);
    char* decoded = decode(encoded);
    printf("Decoded String is:%s",decoded);
}
//make a performance result file for compression
//make header files compression and encryption