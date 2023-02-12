

struct comp_DictNode {
    int value; 
    int prefix; 
    int character; 
    struct comp_DictNode *next;
};


struct comp_DictNode *dictionary, *tail;

void comp_dict_Init();
void comp_appendNode(struct comp_DictNode *node);
void comp_dict_Destroy();
int comp_dict_Lookup(int prefix, int character);
void comp_dict_Add(int prefix, int character, int value);
void comp_writeBinary(FILE * output, int code);
void compress(FILE *inputFile, FILE *outputFile);


