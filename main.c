//Tareq Shannak 1181404 Sec03
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct // Declare a struct for data
{
    char Book[20];
    char Author[20];
    int Date;
    char Address[20];
    char Category[20];
} Data;

//Original code provided by the author Mark Allen Wiess.
//Modified by Dr. Radi Jarrar
//'Delete' Function added by Tareq Shannak
typedef struct AVLnode* AVLNode;
struct AVLnode
{
    Data data;
    AVLNode Left;
    AVLNode Right;
    int Height; //Balance information
};

int IsEmpty(AVLNode T)
{
    return T == NULL;
}

AVLNode MakeEmpty( AVLNode T )
{
    if(!IsEmpty(T))
    {
        MakeEmpty( T->Left );
        MakeEmpty( T->Right );
        free( T );
    }
    return NULL;
}

AVLNode Find( Data D, AVLNode T )
{
    if( IsEmpty(T) )
        return NULL;
    if( strcasecmp(D.Book, T->data.Book) < 0 )
        return Find( D, T->Left );
    else if( strcasecmp(D.Book, T->data.Book) > 0 )
        return Find( D, T->Right );
    else
        return T;
}

int Height( AVLNode P )
{
    if( IsEmpty(P) )
        return -1;
    else
        return P->Height;
}

int Max( int Lhs, int Rhs )
{
    return Lhs > Rhs ? Lhs : Rhs;
}

/* This function can be called only if K2 has a left child */
/* Perform a rotate between a node (K2) and its left child */
/* Update heights, then return new root */

AVLNode SingleRotateWithLeft( AVLNode K2 )
{
    AVLNode  K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = Max( Height( K2->Left ), Height( K2->Right ) ) + 1;
    K1->Height = Max( Height( K1->Left ), K2->Height ) + 1;

    return K1;  /* New root */
}

/* This function can be called only if K1 has a right child */
/* Perform a rotate between a node (K1) and its right child */
/* Update heights, then return new root */

AVLNode SingleRotateWithRight( AVLNode K1 )
{
    AVLNode K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    K1->Height = Max( Height( K1->Left ), Height( K1->Right ) ) + 1;
    K2->Height = Max( Height( K2->Right ), K1->Height ) + 1;

    return K2;  /* New root */
}

/* This function can be called only if K3 has a left */
/* child and K3's left child has a right child */
/* Do the left-right double rotation */
/* Update heights, then return new root */

AVLNode DoubleRotateWithLeft( AVLNode K3 )
{
    /* Rotate between K1 and K2 */
    K3->Left = SingleRotateWithRight( K3->Left );

    /* Rotate between K3 and K2 */
    return SingleRotateWithLeft( K3 );
}
/* END */

/* This function can be called only if K1 has a right */
/* child and K1's right child has a left child */
/* Do the right-left double rotation */
/* Update heights, then return new root */

AVLNode DoubleRotateWithRight( AVLNode K1 )
{
    /* Rotate between K3 and K2 */
    K1->Right = SingleRotateWithLeft( K1->Right );

    /* Rotate between K1 and K2 */
    return SingleRotateWithRight( K1 );
}

void CopyData(Data* D, Data Origin)
{
    strcpy(D->Book, Origin.Book);
    strcpy(D->Author, Origin.Author);
    D->Date = Origin.Date;
    strcpy(D->Address, Origin.Address);
    strcpy(D->Category, Origin.Category);
}

AVLNode Insert( Data D, AVLNode T )
{
    if( IsEmpty(T) )
    {
        /* Create and return a one-node tree */
        T = malloc( sizeof( struct AVLnode ) );
        if( T == NULL )
            printf( "Out of space!\n" );
        else
        {
            CopyData(&T->data, D);
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    }
    else if( strcasecmp(D.Book, T->data.Book) < 0 )
    {
        T->Left = Insert( D, T->Left );
        if( Height( T->Left ) - Height( T->Right ) == 2 )
        {
            if( strcasecmp(D.Book, T->Left->data.Book) < 0 )
                T = SingleRotateWithLeft( T );
            else
                T = DoubleRotateWithLeft( T );
        }
    }
    else if( strcasecmp(D.Book, T->data.Book) > 0 )
    {
        T->Right = Insert( D, T->Right );
        if( Height( T->Right ) - Height( T->Left ) == 2 )
        {
            if( strcasecmp(D.Book, T->Right->data.Book) > 0 )
                T = SingleRotateWithRight( T );
            else
                T = DoubleRotateWithRight( T );
        }
    }
    else
        printf("Sorry, but there's a book with this name!\n");
    T->Height = Max( Height( T->Left ), Height( T->Right ) ) + 1;
    return T;
}

AVLNode MinValueNode(AVLNode T)
{
    AVLNode L = T;
    /* loop down to find the leftmost leaf */
    while (L->Left != NULL)
        L = L->Left;
    return L;
}
// Recursive function to delete a node
// with given D from subtree with
// given root. It returns root of the
// modified subtree.
AVLNode Delete(AVLNode T, Data D)
{

    // STEP 1: PERFORM STANDARD BST DELETE
    if (T == NULL)
    {
        printf("No Book with this name!\n");
        return T;
    }

    // If the D to be deleted is smaller
    // than the T's D, then it lies
    // in left subtree
    if(strcasecmp(D.Book,T->data.Book) < 0)
        T->Left = Delete(T->Left, D);

    // If the D to be deleted is greater
    // than the T's D, then it lies
    // in right subtree
    else if(strcasecmp(D.Book,T->data.Book) > 0)
        T->Right = Delete(T->Right, D);

    // if D is same as T's D, then
    // This is the node to be deleted
    else
    {
        // node with only one child or no child
        if( (T->Left == NULL) || (T->Right == NULL) )
        {
            AVLNode temp = T->Left ? T->Left : T->Right;

            // No child case
            if (temp == NULL)
            {
                temp = T;
                T = NULL;
            }
            else // One child case
                *T = *temp; // Copy the contents of
            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            AVLNode temp = MinValueNode(T->Right);

            // Copy the inorder successor's
            // data to this node
            CopyData(&T->data, temp->data);
            // Delete the inorder successor
            T->Right = Delete(T->Right, temp->data);
        }
        printf("The book with this name deleted!\n");
    }

    // If the tree had only one node
    // then return
    if (T == NULL)
        return T;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    T->Height = 1 + Max(Height(T->Left), Height(T->Right));

    // STEP 3: GET THE BALANCE FACTOR OF
    // THIS NODE (to check whether this
    // node became unbalanced)
    int balance = Height( T->Left ) - Height( T->Right );

    // If this node becomes unbalanced,
    // then there are 4 cases

    // Left Left Case
    if (balance > 1 &&
            Height( T->Left->Left ) - Height( T->Left->Right ) >= 0)
        return SingleRotateWithRight(T);

    // Left Right Case
    if (balance > 1 &&
            Height( T->Left->Left ) - Height( T->Left->Right ) < 0)
    {
        T->Left = SingleRotateWithLeft(T->Left);
        return SingleRotateWithRight(T);
    }

    // Right Right Case
    if (balance < -1 &&
            Height( T->Right->Left ) - Height( T->Right->Right ) <= 0)
        return SingleRotateWithLeft(T);

    // Right Left Case
    if (balance < -1 &&
            Height( T->Right->Left ) - Height( T->Right->Right ) > 0)
    {
        T->Right = SingleRotateWithRight(T->Right);
        return SingleRotateWithLeft(T);
    }
    return T;
}

void DisposeAVL(AVLNode T)
{
    if(!IsEmpty(T))
    {
        DisposeAVL(T->Left);
        free(T);
        DisposeAVL(T->Right);
    }
}

void PrintFiction( AVLNode T)
{
    if(!IsEmpty(T))
    {
        PrintFiction( T->Left );
        if(strcasecmp(T->data.Category,"Fiction") == 0)
        {
            printf("\n%s %s %d %s", T->data.Book, T->data.Author, T->data.Date, T->data.Address);
        }
        PrintFiction( T->Right );
    }
}

void PrintNonFiction( AVLNode T)
{

    if(!IsEmpty(T))
    {
        PrintNonFiction( T->Left );
        if(strcasecmp(T->data.Category,"Non-Fiction") == 0 || strcasecmp(T->data.Category,"Non Fiction") == 0)
        {
            printf("\n%s %s %d %s", T->data.Book, T->data.Author, T->data.Date, T->data.Address);
        }
        PrintNonFiction( T->Right );
    }
}

//Functions..
void printMenu() // Just Print..
{
    printf("------------------- Menu -------------------\n");
    printf("Write the number of the option that you want\n\n");
    printf("         1. Read the data in the file.\n");
    printf("       2. Add a new book to the library.\n");
    printf("      3. Search for a book in the library.\n");
    printf("     4.  Update or delete a specific book .\n");
    printf("    5. Show the books in lexicographic order.\n");
    printf("      6. Print the height of the AVL tree.\n");
    printf("         7. Save the data to the file.\n");
    printf("           8. Exit the application.\n");
}

int Confirmed(char ConfirmOption[]) // print a confirm message and scan the input
{
    printf("\tAre you sure to complete this process?\n\t- Press any key except 1 to cancel -\n\t    1. Yes\n");
    scanf("%s", ConfirmOption);
    if(atoi(ConfirmOption) == 1 && strlen(ConfirmOption) == 1) // if the input is 1 return true, else return false
        return 1;
    return 0;
}

char* Trim(char *S) // Remove the white spaces
{
    int i, j;
    for(i=0; S[i]==' '||S[i]=='\t'; i++); // to make 'i' equals the first UnSpace character's Location

    for(j=0; S[i]; i++)
    {
        S[j++]=S[i]; // to copy the string without the left white space
    }
    S[j]='\0'; // insert the null character
    for(i=0; S[i]!='\0'; i++) // while 'i' doesn't equal the null character
    {
        if(S[i]!=' '&& S[i]!='\t') // reach the last UnSpace character's location
            j=i;                  // and put it in 'j'
    }
    S[j+1]='\0'; // insert the null character after the last UnSpace character
    return S;
}

void ReadFile(AVLNode *T, Data* D) // Read books.txt File
{
    char temp[100]; // To store the scanned line
    *T = MakeEmpty(*T) ; // to provide the repeated scanning..
    FILE* in = fopen("books.txt","r");
    if(in == NULL)
    {
        printf("Error in opening the file!\n");
        return;
    }
    while(fscanf(in, "\n%[^\n]s", temp)!= EOF) // if there is a line to scan..
    {
        // split by "|",remove the white spaces and store them in a suitable place..
        strcpy(D->Book, Trim(strtok(temp,"|")));
        strcpy(D->Author, Trim(strtok(NULL,"|")));
        D->Date = atoi(Trim(strtok(NULL,"|")));
        strcpy(D->Address, Trim(strtok(NULL,"|")));
        strcpy(D->Category, Trim(strtok(NULL,"\n")));
        if(D->Date > 0 && (strcasecmp(D->Category, "Non-Fiction") == 0
                           || strcasecmp(D->Category, "Non Fiction") == 0 || strcasecmp(D->Category, "Fiction") == 0)) // To check that the inputs are correct
            *T = Insert( *D, *T);
        else
        {
            // If the inputs are incorrect, print this message
            printf("       There is at least one fault in your inputs\n");
            printf("  The book information must be written like this:\n");
            printf(" Algorithms | Mark Weiss | 1996 | USA | Non-fiction\n");
            printf(" Please correct the inputs to be as the previous form!\n\n");
            *T = MakeEmpty(*T);
            return; // break the while-loop reading
        }
    }
    if(IsEmpty(*T))
        printf("File is Empty!\n");
    else
        printf("The Input file has been read correctly!\n");
    fclose(in);
}

void ScanBook(Data *D, int flag) // To Scan the inputs from user (Not from File)
{
    if(flag){ // sometime we don't need scanning the book's name
    printf("Enter Book's Name:\n");
    scanf("\n%[^\n]s", D->Book);
    }
    printf("Enter Author's Name:\n");
    scanf("\n%[^\n]s", D->Author);
    printf("Enter the Publishing Date:\n");
    scanf("%d", &D->Date);
    printf("Enter the Publisher Address:\n");
    scanf("\n%[^\n]s", D->Address);
    printf("Enter the Category:\n");
    scanf("\n%[^\n]s", D->Category);
}

void WriteFile(AVLNode T, FILE* out) // Write books.txt File
{
    if(T != NULL)
    {
        WriteFile(T->Left, out); // Print Data in the file
        fprintf(out,"%s | %s | %d | %s | %s\n", T->data.Book, T->data.Author, T->data.Date, T->data.Address, T->data.Category);
        WriteFile(T->Right, out);
    }
}

void DisplayAuthor(Data D, AVLNode T, int* flag) // Print author's Books
{
    if(!IsEmpty(T))
    {
        DisplayAuthor(D, T->Left, flag);
        if(strcasecmp(T->data.Author,D.Author) == 0)
        {
            printf("%s | %s | %d | %s | %s\n", T->data.Book, T->data.Author, T->data.Date, T->data.Address, T->data.Category);
            *flag = 1;
        }
        DisplayAuthor(D, T->Right, flag);
    }
}

int main()
{
    AVLNode tree = NULL;   // Create AVL tree that we will use in functions by passing it
    tree = MakeEmpty(tree);
    Data D ; // Create a struct Data that we will need it to store the data temporary in some operations
    char Option[2], TempOption[2]; // To Scan the option from user
    do // do the statements below at least once
    {
        printMenu(); // call function to print menu
        scanf("%s", Option);
        if(strlen(Option) > 1) // because the options have one character
            strcpy(Option,"0") ;
        switch(atoi(Option)) // switch to know what is the option
        {
        case 1 : // if Option is 1..
            if(!IsEmpty(tree)) // if there are a held data ..
            {
                printf("The currently held library will be lost\n"); // print a message
                if(Confirmed(TempOption)) // and ask to confirm this process
                    ReadFile(&tree, &D); // Call function to read
            }
            else  // if there is no held data
                ReadFile(&tree, &D); // Call function to read
            break;
        case 2 : // if Option is 2..
            ScanBook(&D, 1); // take data from user for the new book
            if(D.Date > 0 && (strcasecmp(D.Category, "Non-Fiction") == 0  // To check that the inputs are correct
                              || strcasecmp(D.Category, "Non Fiction") == 0 || strcasecmp(D.Category, "Fiction") == 0))
            {
                tree = Insert(D, tree); // insert the new book to library
                printf("The book has been added!\n");
            }
            else
                printf("Wrong Input!\n");
            break;
        case 3 : // if Option is 3..
            if(IsEmpty(tree)) // if there is no held data..
            {
                printf("\tThere is no books to search!\n"); // print this message
                break; // and don't continue
            } // if there are held data..
            printf("--------------------------------------------\n\n");   //  print this message to select
            printf("\tWrite the number of the option that you want\n");    // which type of searching we will use
            printf("1. Search using Book's Name    2. Search using Author's Name\n");
            scanf("%s",TempOption);
            if((atoi(TempOption) == 1 || atoi(TempOption) == 2) && strlen(TempOption) == 1) // if option is 1 or 2..
            {
                if(atoi(TempOption) == 1) // if option is 1..
                {
                    printf("Enter Book's Name:\n"); // Scan the book's name
                    scanf("\n%[^\n]s", D.Book);
                    if(Find(D, tree) != NULL) // if the book exists..
                    {
                        CopyData(&D, Find(D, tree)->data); // Copy the data to D & print it
                        printf("%s | %s | %d | %s | %s\n", D.Book, D.Author, D.Date, D.Address, D.Category);
                    }
                    else
                        printf("Can't Found!\n");
                }
                else // if option is 2..
                {
                    printf("Enter Author's Name:\n"); // Scan author's name
                    scanf("\n%[^\n]s", D.Author);
                    int flag = 0;
                    DisplayAuthor(D, tree, &flag); // print his book/s
                    if(!flag) // if there's no books for this author, print a message
                        printf("Can't Found!\n");
                }
            }
            else // if the scanned option is not 1 or 2 ..
                printf("     Sorry but your input is not correct!\n"); // print this message and break
            break;
        case 4 : // if Option is 4..
            if(IsEmpty(tree)) // if there is no held data..
            {
                printf("\tThere is no books to update!\n"); // print this message
                break; // and don't continue
            } // if there are held data..
            printf("--------------------------------------------\n\n");   //  print this message to select
            printf("Write the number of the option that you want\n");
            printf("1. Update a book  \t  2. Delete a book\n");
            scanf("%s",TempOption);
            if((atoi(TempOption) == 1 || atoi(TempOption) == 2) && strlen(TempOption) == 1) // if option is 1 or 2..
            {
                printf("Enter Book's Name:\n"); // Scan Book's name
                scanf("\n%[^\n]s", D.Book);
                if(Find(D, tree) != NULL) // if the book is exists..
                {
                    if(atoi(TempOption) == 1) // Update if the option is 1
                    {
                        // scan the information from user
                        ScanBook(&D, 0);
                        if(D.Date > 0 && (strcasecmp(D.Category, "Non-Fiction") == 0 // To check that the inputs are correct
                                || strcasecmp(D.Category, "Non Fiction") == 0 || strcasecmp(D.Category, "Fiction") == 0))
                        {
                            CopyData(&Find(D, tree)->data, D); // Put the new data..
                            printf("Book information updated!\n");
                        }
                        else
                            printf("Wrong Input!\n");
                    }
                    else  // Delete
                        tree = Delete(tree, D);
                }
                else
                    printf("Can't Found!\n");
            }
            else  // if the scanned option is not 1 or 2 ..
                printf("     Sorry but your input is not correct!\n"); // print this message and break
            break;
        case 5 : // if Option is 5..
            if(IsEmpty(tree))
            {
                printf("\tNo Books to show!\n");
                break;
            }
            printf("\nFiction Books: "); // Print in-order..
            PrintFiction(tree);
            printf("\nNon-Fiction Books: ");
            PrintNonFiction(tree);
            printf("\n");
            break;
        case 6 : // if Option is 6..
            if(Height(tree) != -1)
                printf("The height : %d\n", Height(tree));
            else
                printf("\tThe tree is Empty!\n");
            break;
        case 7 : // Save Data..
            if(IsEmpty(tree)) // if no data print a message
            {
                printf("There is nothing to save!\n"); // print a message to explain}
                break;
            }
            FILE* out = fopen("books.txt","w");
            WriteFile(tree, out); // call function to write
            fclose(out);
            printf("The data are saved!\n");
            break;
        case 8 :
            break;
        default: // if the previous options don't include the scanned option
            printf("Sorry but this thing does not exist!\n");
        }
    }
    while(atoi(Option) != 8); // if option 8 had not chosen, do the do-while loop one more time
    DisposeAVL(tree); // At the end, free the tree
    return 0;
}
