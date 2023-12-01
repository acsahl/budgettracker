//
//  main.c
//  Budget Project
//
//  Created by Acsah Lukose on 11/11/23.
//

//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
struct budget_struct {
    char *name;
    char **categories;
    int categoriesNum;
    double* budgetValues;
    
    double max;
    double min;
    double total;
    
};
typedef struct budget_struct budget;

double budget_max(budget* b) {
    double max = 0;
    for (int i = 0; i < (*b).categoriesNum; i++) {
        for (int j = 0; j < 5; j++) {
            if ((*b).budgetValues[j * (*b).categoriesNum + i] > max) {
                max = (*b).budgetValues[j * (*b).categoriesNum + i];
            }
        }
    }
    return max;
}

double budget_min(budget* b) {
    double min = DBL_MAX;
    for (int i = 0; i < (*b).categoriesNum; i++) {
        for (int j = 0; j < 5; j++) {
            if ((*b).budgetValues[j * (*b).categoriesNum + i] < min) {
                min = (*b).budgetValues[j * (*b).categoriesNum + i];
            }
        }
    }
    return min;
}

double budget_tot(budget* b) {
    double total = 0;
    for (int i = 0; i < (*b).categoriesNum; i++) {
        for (int j = 0; j < 5; j++) {
            total = total + (*b).budgetValues[j * (*b).categoriesNum + i];
        }
    }
    return total;
}

void give_advice(budget*b, int catNum, double percent) {
    double total = (*b).total;
    double catTotal = 0;
    for (int i = 0; i < 5; i++) {
        catTotal = catTotal + (*b).budgetValues[(catNum-1) * (*b).categoriesNum + i];
    }
    double currentPercent = ((catTotal/total)*100);
    
    printf("Your current percentage of the listed Category # is %.2lf \n", currentPercent);
    
    double needAmt = ((percent * total)/100);
    double difference = 0;
    
    if (needAmt > catTotal) {
        difference = needAmt - catTotal;
        printf("You have to increase the money in this category by %.2lf dollars \n", difference);
    } else if (needAmt < catTotal) {
        difference = catTotal - needAmt;
        printf("You have too much money in this cateogry. Take out %.2lf dollars \n", difference);
    } else {
        printf("Congratulations! You've already met that goal! Maintain your current amount. \n");
    }
    
}

void print_budget(budget *b) {
    printf("Budget: %s \n", (*b).name);
    printf("\n");
    for(int i = 0; i < (*b).categoriesNum; i++) {
        printf("%s:   ", (*b).categories[i]);
        for (int j = 0; j < 5; j++) {
            printf("%.2lf    ", (*b).budgetValues[j * (*b).categoriesNum + i]);
        }
        printf("\n");
    }
    
    
}

void edit_budget(budget *b, int catNum, int value, double newNum) {
    int valueInArr = value - 1;
    int catInArr = catNum - 1;
    (*b).budgetValues[valueInArr * (*b).categoriesNum + catInArr] = newNum;
}
budget *new_budget(char **categories, double* budgetValues, int categoriesNum, char *name) {
    budget *b= malloc(sizeof(budget));
    
    b -> categories = categories;
    b -> budgetValues = budgetValues;
    b -> categoriesNum = categoriesNum;
    b -> name = name;
    
    b -> max = budget_max(b);
    b -> min = budget_min(b);
    b -> total = budget_tot(b);
    return b;
    
}



void dispose_budget(budget *budget_obj) {
    free(budget_obj);
}

//non-struct reliant funtions
void printMenu(void) {
    printf("max - Find the maximum amongst all categories \n");
    printf("min - Find the minimum amongst all categories \n");
    printf("tot - Find the total amongst all categories \n");
    printf("giv - Give advice based on percentages... \n");
    printf("prt - Print out your current budget \n");
   printf("edit - Edit any value! \n");
    printf("save - Save the Values in the Budget. \n");
    printf("ext - Exit the program \n");
    printf("Enter choice here: ");
}

int main(void) {
    FILE *ifp;
    FILE *ofp;
    char name[100];
    int categoriesNum = 0;
    double* budgetPtr = NULL;

    char *namePtr;
    char **categories;
    
    ifp = fopen("/Users/acsah/Desktop/budget.txt", "r");
    if (ifp == NULL) {
        printf("Could not locate existing budget file. Please create one now! \n");
        printf("Type in your name for the Budget \n");
        scanf("%s", name);
        namePtr = name;
        
        printf("Type in the number of categories you have (1-5): \n");
        scanf("%d", &categoriesNum);
        //creating categories array
        categories = malloc(categoriesNum * sizeof(char*));
        for (int i = 0; i < categoriesNum; i++) {
            categories[i] = malloc(100 * sizeof(char));
        }
        

        for (int i = 0; i < categoriesNum; i++) {
            printf("Enter name for Category # %d: \n", (i + 1));
            scanf("%s", categories[i]);
        }
        
        //test categories
        /*
        for(int i = 0; i < categoriesNum; i++) {
            printf("Category: %s\n", &categories[i]);
        }
         */
        // printf("Enter Value for Category: %s Space: %d \n", &categories[i], j + 1);
        // scanf("%lf", &budgetValues[i][j]);
        budgetPtr = malloc((5 * categoriesNum) * sizeof(double));
        printf("You are allotted 5 values per category. Enter 0 if all 5 aren't needed. \n");
        
        for (int i = 0; i < categoriesNum; i++) {
            for (int j = 0; j < 5; j++) {
                printf("Enter Value for Category: %s Space: %d \n", categories[i], j + 1);
                scanf("%lf", &budgetPtr[j * categoriesNum + i]);
            }
            
        }
    } else {
        printf("Found pre-existing file. Will now use information for budget... \n");
        fscanf(ifp,"%s", name);
        namePtr = name;
        fscanf(ifp, "%d", &categoriesNum);
        budgetPtr = malloc((5 * categoriesNum) * sizeof(double));
        categories = malloc(categoriesNum * sizeof(char*));
        for (int i = 0; i < categoriesNum; i++) {
            categories[i] = malloc(100 * sizeof(char));
        }
        for(int i = 0; i < categoriesNum; i++) {
            fscanf(ifp, "%s", categories[i]);
            fscanf(ifp, "%lf %lf %lf %lf %lf", &budgetPtr[0 * categoriesNum + i], &budgetPtr[1 * categoriesNum + i], &budgetPtr[2 * categoriesNum + i], &budgetPtr[3 * categoriesNum + i], &budgetPtr[4 * categoriesNum + i]);
        }
        
    }
    fclose(ifp);
    
   
    
    
    budget *budget_test = new_budget(categories, budgetPtr, categoriesNum, namePtr);
    
    char output[5] = "test";
    printf("Now to proceed with the menu... \n");
    
    while (strcmp(output, "ext") != 0) {
        printMenu();
        scanf("  %s", output);
        if (strcmp(output, "max") == 0) {
            printf("The maximum of all the values is %.2lf \n", budget_test -> max);
        } else if (strcmp(output, "min") == 0) {
            printf("The minimum of all the values is %.2lf \n", budget_test -> min);
        } else if (strcmp(output, "tot") == 0) {
            printf("The total of all the values is %.2lf \n", budget_test -> total);
        } else if (strcmp(output, "giv") == 0) {
            int catNum;
            double percent;
            printf("Which Category # Do you wish to get advice for (refer to initial input) \n");
            scanf("%d", &catNum);
            printf("Which percentage are you aiming for, compared to total? \n");
            scanf("%lf", &percent);
            give_advice(budget_test,catNum, percent);
        } else if (strcmp(output, "prt") == 0) {
            print_budget(budget_test);
        } else if (strcmp(output, "ext" ) == 0) {
            printf("Thank you for joining! Come again soon!");
        } else if (strcmp(output, "save") == 0) {
            ofp = fopen("/Users/acsah/Desktop/budget.txt", "w");
            fprintf(ofp, "%s \n", namePtr);
            fprintf(ofp, "%d", categoriesNum);
            fprintf(ofp, "\n");
            for(int i = 0; i < budget_test -> categoriesNum; i++) {
                fprintf(ofp, "%s  \n", budget_test -> categories[i]);
                for (int j = 0; j < 5; j++) {
                    fprintf(ofp, "%.2lf ", budget_test -> budgetValues[j * budget_test -> categoriesNum + i]);
                }
                fprintf(ofp, "\n");
               
            }
            fclose(ofp);
           printf("Values saved!! \n");
        } else if (strcmp(output, "edit") == 0) {
           int catNum;
           int valueNum;
           double newValue;
           printf("What Category is the Value you would like to edit in (refer to ID with category? \n");
           scanf("%d", &catNum);
           printf("Which slot is the value you would like to replace in? (1-5) \n");
           scanf("%d", &valueNum);
           printf("Finally, what is the new value you would like to enter in? \n");
           scanf("%lf", &newValue);
           edit_budget(budget_test, catNum, valueNum, newValue);
        } else {
            printf("Invalid Input. Try Again. \n");
        }
    }
    
    
    dispose_budget(budget_test);
    
    
}
