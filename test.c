#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct customer{
    long long int telp_num;
    char name[30];
    char email[30];
    int membership_point;
    struct customer *to_lower;
    struct customer *to_greater;
}cust;

typedef struct order{
    char drink_name[20];
    char quantity;
}order;

cust *root;
int data_count=0;

void print_menu(){
    printf("BlueBucks\n");
    printf("==================\n");
    printf("1. Process Order\n");
    printf("2. View All Customer\n");
    printf("3. Remove Customer\n");
    printf("4. Exit\n>>");
    return;
}

void clear(){
    system("pause");
    system("cls");
}

cust* search_key(cust *head, long long int *inp_key){
    if(head == NULL){
        return NULL;
    }
    if(head->telp_num == *inp_key){
        return head;
    }
    if(head->telp_num > *inp_key){
        return search_key(head->to_lower, inp_key);
    }
    else if(head->telp_num < *inp_key){
        return search_key(head->to_greater, inp_key);
    }
    return NULL;
}

void add_data(cust *newnode){
    if(data_count==0){
        root = newnode;
        data_count++;
        return;
    }else{
        cust *curr = root;
        cust *parent;
        while(true){
            parent = curr;
            if(newnode->telp_num>curr->telp_num){
                curr = curr->to_greater;
                if(curr==NULL){
                    parent->to_greater=newnode;
                    break;
                }
            }else if(newnode->telp_num<curr->telp_num){
                curr = curr->to_lower;
                if(curr==NULL){
                    parent->to_lower=newnode;
                    break;
                }
            }else{
                continue;
            }
        }
        data_count++;
        return;
    }
}

void process_order(){
    cust *inp_cust=malloc(sizeof(cust));
    char input_pn[13]; bool phone_status = false, gender = false;

    while(true){ //phone number inputa
        printf("Input phone number[10-13][numeric]: ");
        scanf("%[^\n]", input_pn);getchar();
        if(9<strlen(input_pn) && strlen(input_pn)<14){
            for(int i=0; input_pn[i]!='\0'; i++){
                if(isdigit(input_pn[i])){
                    phone_status = true;
                    continue;
                }else{
                    phone_status = false;
                    break;
                }
            }
        }
        if(phone_status){
            inp_cust->telp_num = atoll(input_pn);
            break;
        }else{
            printf("Invalid\n");
        }
    }

    if(!search_key(root, &inp_cust->telp_num)){
        while(true){ //name input
            printf("Input name[3 - 25][Mr. |Mrs. ]: ");
            scanf("%[^\n]", inp_cust->name);getchar();
            if(strlen(inp_cust->name)>25 || strlen(inp_cust->name)<4){
                continue;
            }
            if(strncmp(inp_cust->name, "Mr. ", 4) == 0){
                gender = true;
                break;
            }else if(strncmp(inp_cust->name, "Mrs. ", 5) == 0){
                gender = false;
                break;
            }else{
                continue;
            }
        }   

        while(true){ //email input
            printf("Input email[10-20][username@email.com | username@email.co.id]:");
            scanf("%[^\n]", inp_cust->email); getchar();
            if (10 <= strlen(inp_cust->email) && strlen(inp_cust->email) <= 20){
                bool check_email = true; int Acount = 0;
                for(int i=0; inp_cust->email[i]!='\0'; i++){
                    if(inp_cust->email[i]=='@'){
                        Acount++;
                        if(Acount>1){
                            printf("1");
                            check_email = false;
                            break;
                        }
                    }
                }
                if(!check_email){
                    printf("Invalid Email\n");
                    continue;
                }
                if(strstr(inp_cust->email, ".com")){
                    check_email = true;
                    break;
                }else if(strstr(inp_cust->email, ".co.id")){
                    check_email = true;
                    break;
                }else{
                    check_email = false;
                    continue;
                }
            }else{
                continue;
            }
        }
        inp_cust->membership_point  = 10;
        inp_cust->to_greater = NULL;
        inp_cust->to_lower = NULL;
        add_data(inp_cust);
        printf("Insert Success\n");
    }

    order orderTotal[30]; int orderIndex=0;
    int total_price=0, price = 30000;int point_used=0;
    cust *curr = search_key(root, &inp_cust->telp_num);
    fflush(stdin);

    if(curr->membership_point>=25){ //use point for free drink  
        while(true){
            printf("Do you want to use your points[You have %d][y|n]: ", curr->membership_point);
            char choice;scanf("%c", &choice);getchar();
            if(choice == 'y'){
                while(true){
                    printf("How much[%d left]: ",curr->membership_point);
                    scanf("%d", &point_used);getchar();
                    if(point_used%25==0 && point_used<=curr->membership_point){
                        break;
                    }else{
                        continue;
                    }
                }
                break;
            }else if(choice == 'n'){
                break;
            }else{
                continue;
            }
        }
    }

    int free_drink=point_used/25;
    printf("\nYou have %d free drink(s) left\n\n", free_drink);

    while(true){ //order input
        char yn;
        while(true){ //drink name input
            printf("Input drink[Cafe Latte|Caramel Macchiato|Cappucino|Cafe Mocha]: ");
            scanf("%[^\n]", orderTotal[orderIndex].drink_name); getchar();
            if((!strcmp(orderTotal[orderIndex].drink_name, "Cafe Latte")) || (!strcmp(orderTotal[orderIndex].drink_name, "Caramel Macchiato")) || (!strcmp(orderTotal[orderIndex].drink_name, "Cappucino")) || (!strcmp(orderTotal[orderIndex].drink_name, "Cafe Mocha"))){
                break;
            }
        }
               
        while(true){ //drink quantity input
            printf("Input drink quantity[>=1]: ");
            scanf("%d", &orderTotal[orderIndex].quantity); getchar();
            if(orderTotal[orderIndex].quantity>=1){
                break;
            }
        }
        
        while(true){ //continue/no?
            printf("Do you want to order more[y|n]: ");
            scanf("%c", &yn); getchar();system("cls");
            if(yn=='y' || yn=='n'){
                break;
            }
        }
        if(yn=='n'){ //break loop condition
            break;
        }
        orderIndex++;
    }

    printf("Order Summary:\n");
    for(int i=0; i<=orderIndex; i++){ //order summary print
        printf("%-20s - %dx = %d\n", orderTotal[i].drink_name, orderTotal[i].quantity, price*orderTotal[i].quantity);
        total_price = total_price + price*orderTotal[i].quantity;
    }
    printf("Membership Free Drink Discount: %d\n", free_drink*price);
    printf("Total: %d\n", total_price-free_drink*price);
    int point_obtained = (total_price/50000)*3;
    printf("Points Obtained: %d\n", point_obtained);
    curr->membership_point += point_obtained;
    return;
}

void customer_table_header(){
    printf("==================================================================================\n");
    printf("| Name                      | Phone Number  | Email                     | Points |\n");
    printf("==================================================================================\n");
    return;
}

void view_customer_list(cust *head){
    if(head==NULL){
        return;
    }

    view_customer_list(head->to_lower);
    printf("| %-25s | %-13lld | %-25s | %-6d |\n", head->name, head->telp_num, head->email, head->membership_point);
    printf("----------------------------------------------------------------------------------\n");
    view_customer_list(head->to_greater);
    return;
}

cust* min_value_node(cust *node) {
    cust *current = node;
    while (current && current->to_lower != NULL) {
        current = current->to_lower;
    }
    return current;
}

cust* remove_customer(cust *head, long long int input_phoneNum) {
    if (head == NULL) {
        return head;
    }

    if (input_phoneNum < head->telp_num) {
        head->to_lower = remove_customer(head->to_lower, input_phoneNum);
    }
    else if (input_phoneNum > head->telp_num) {
        head->to_greater = remove_customer(head->to_greater, input_phoneNum);
    }
    else {
        if (head->to_lower == NULL) {
            cust *temp = head->to_greater;
            free(head);
            return temp;
        }
        else if (head->to_greater == NULL) {
            cust *temp = head->to_lower;
            free(head);
            return temp;
        }
        cust *temp = min_value_node(head->to_greater);
        head->telp_num = temp->telp_num;
        head->to_greater = remove_customer(head->to_greater, temp->telp_num);
        free(temp);
    }
    return head;
}

cust* deleteNode(cust* head, long long int key) {
    // base case
    if (head == NULL) return head;

    if (key < head->telp_num)
        head->to_lower = deleteNode(head->to_lower, key);

    else if (key > head->telp_num)
        head->to_greater = deleteNode(head->to_greater, key);
    else {
        if (head->to_lower == NULL) {
            cust* temp = head->to_greater;
            free(head);
            return temp;
        }
        else if (head->to_greater == NULL) {
            cust* temp = head->to_greater;
            free(head);
            return temp;
        }

        cust* temp = min_value_node(head->to_greater);
        head->telp_num = temp->telp_num;
        head->to_greater = deleteNode(head->to_greater, temp->telp_num);
    }
    return head;
}

void main_menu(){
    while(true){
        print_menu();
        int ch;scanf("%d", &ch); getchar();
        system("cls");
        if(ch==1){
            process_order();
            clear();
        }

        else if(ch==2){
            system("cls");
            if(!data_count){
                printf("There's no data !\n");
                clear();
                continue;
            }
            customer_table_header();
            view_customer_list(root);
            clear();
        }

        else if(ch==3){
            customer_table_header();
            view_customer_list(root);
            printf("Input phone number: ");
            long long int input; scanf("%lld", &input);getchar();
            remove_customer(root, input);
            data_count--;
            clear();
        }

        else if(ch==4){
            exit(0);
        }

        else{
            continue;
        }
    }
}

int main(){
    main_menu();
    return 0;
}
