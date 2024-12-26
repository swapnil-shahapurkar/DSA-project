#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <time.h>

#define NO_OF_CHARS 256
#define INT_MAX 1000
#define MAX_NODES 51

int adj[1000][1000], acount = 0, ccount = 0, i, j;
int dist[1000][1000], dcount = 0;
int hcount = 0, lcount = 0, rcount = 0;

typedef struct cities
{
    int id;
    char name[50];
} CT;

CT S[1000];

struct hotel
{
    char name[50];
    char place[50];
    float rating;
    int price;
    long long int phoneNumber;
    char mail[50];
} ht[100];

struct lodge
{
    char name[50];
    char location[50];
    int price;
    float rating;
    int stars;
    long long int phoneNumber;
} lo[100];

struct restaurants
{
    char name[50];
    char place[50];
    char type[10];
    float rating;
    char range[50];
    long long int phoneNumber;
} re[100];

typedef struct User
{
    char username[50];
    char password[50];
    char fav_location[50];
    char fav_restaurant[50];
    char fav_lodge[50];
} User;

int calorder()
{
    FILE *fp = fopen("adjacency_matrix.txt", "r");

    char ch;
    int row = 0;

    if (fp == NULL)
    {
        printf("\n cannot open the file");
        return 0;
    }

    while ((ch = fgetc(fp)) != '\n')
    {
        if (ch == ',')
        {
            row++;
        }
    }

    return row;
    fclose(fp);
}

void load_distance()
{

    FILE *fp;
    fp = fopen("distance_matrix.txt", "r");

    int n = calorder();
    int temp;
    char s;

    dcount = n;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            fscanf(fp, "%d%c", &temp, &s);
            dist[i][j] = temp;
        }
    }

    fclose(fp);
    return;
}

void load_adjacency()
{
    FILE *fp;
    fp = fopen("adjacency_matrix.txt", "r");
    int n = calorder();
    int temp;
    char s;

    acount = n;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            fscanf(fp, "%d%c", &temp, &s);
            adj[i][j] = temp;
        }
    }

    fclose(fp);
    return;
}

void load_cities()
{
    FILE *city;
    city = fopen("cities_list.txt", "r");
    rewind(city);
    int idd;
    char nam[30];

    if (city == NULL)
    {
        printf("\n Cannot open the file");
        return;
    }
    while (!feof(city))
    {
        fscanf(city, "%d%s", &idd, nam);
        S[ccount].id = idd;
        strcpy(S[ccount].name, nam);
        ccount++;
    }

    fclose(city);
    return;
}

int minDistance(int dist[], int sptSet[])
{
    int min = INT_MAX, min_index;
    int v;

    for (v = 0; v < dcount; v++)
    {
        if (sptSet[v] == 0 && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void printPath(int parent[], int j)
{
    if (parent[j] == -1)
    {
        return;
    }

    printPath(parent, parent[j]);
    printf(" --> (%d km) --> %s", dist[parent[j]][j], S[j].name);
}

void printSolution(int dist[], int parent[], int src, int dest)
{
    int hr, min, speed;
    float time;

    printf("\n   Enter the time of departure:\n");
    printf("   Hour: ");
    scanf("%d", &hr);
    printf("   Minute:");
    scanf("%d", &min);
    printf("   Enter the average speed of travelling(km/hr): ");
    scanf("%d", &speed);

    time = dist[dest] / (float)speed;

    printf("\n   Shortest distance from source to destination: %d km\n", dist[dest]);

    int rhr = hr + (int)(ceil(time));
    if (rhr > 24)
        rhr -= 24;

    printf("\n   You will reach your destination before %d:%d\n", rhr, min);
    printf("\n   Shortest path: %s", S[src].name);
    printPath(parent, dest);
    printf("\n");
}

void dijkstra(int src, int dest)
{
    int dist[MAX_NODES];
    int sptSet[MAX_NODES];
    int parent[MAX_NODES];
    int i;
    for (i = 0; i < dcount; i++)
    {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
        parent[i] = -1;
    }

    dist[src] = 0;
    int count;

    for (count = 0; count < dcount - 1; count++)
    {
        int u = minDistance(dist, sptSet);
        sptSet[u] = 1;
        int v;
        for (v = 0; v < dcount; v++)
        {
            if (!sptSet[v] && adj[u][v] && dist[u] != INT_MAX && dist[u] + adj[u][v] < dist[v])
            {
                dist[v] = dist[u] + adj[u][v];
                parent[v] = u;
            }
        }
    }

    printSolution(dist, parent, src, dest);
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void badchartable(char *str, int size, int badchar[NO_OF_CHARS])
{
    int i;
    for (i = 0; i < NO_OF_CHARS; i++)
    {
        badchar[i] = -1;
    }
    for (i = 0; i < size; i++)
    {
        badchar[(int)str[i]] = i;
    }
}

int checklist(char key[])
{
    int flag = 0;

    for (i = 0; i < ccount; i++)
    {
        if (strcmpi(key, S[i].name) == 0)
            return S[i].id;
    }
    for (i = 0; i < ccount; i++)
    {
        int m = strlen(key);
        int n = strlen(S[i].name);
        int badchar[NO_OF_CHARS];

        badchartable(key, m, badchar);

        int s = 0;
        while (s <= (n - m))
        {
            int j = m - 1;
            while (j >= 0 && key[j] == S[i].name[s + j])
            {
                j--;
            }

            if (j < 0)
            {
                printf("\n   Did you mean = %s", S[i].name);
                flag = 1;
                break;
            }
            else
            {
                s += max(1, j - badchar[S[i].name[s + j]]);
            }
        }
    }
    if (flag == 0)
    {
        printf("\n   The city is not enlisted\n");
    }
    return 1000;
}

void KTM()
{
    char source[50], desti[50];

lable:
    printf("\n   Enter the city you are traveling from (Source): ");
    scanf("%s", source);
    if (checklist(source) == 1000)
    {
        goto lable;
    }

renter:
    printf("\n   Enter the city you are traveling to (Destination): ");
    scanf("%s", desti);
    if (checklist(desti) == 1000)
    {
        goto renter;
    }

    dijkstra(checklist(source), checklist(desti));
}

void replaceCharWithSpace(char str[])
{
    int n = strlen(str);

    for (i = 0; i < n; i++)
    {
        if (str[i] == '-')
        {
            str[i] = ' ';
        }
    }
}

void load_hotels(char key[])
{
    FILE *fp;
    struct hotel temp;

    fp = fopen("hotels.txt", "r");
    if (fp == NULL)
    {
        printf("\n Cannot open the file");
        return;
    }

    printf("\n--------------------------------------- Hotels in %s ----------------------------------------------------\n", key);
    printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
    printf("|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n");
    printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");

    while (fscanf(fp, "%s%s%f%d%lld%s", temp.name, temp.place, &temp.rating, &temp.price, &temp.phoneNumber, temp.mail) == 6)
    {
        if (strcmpi(key, temp.place) == 0)
        {
            ht[hcount] = temp;
            replaceCharWithSpace(temp.name);
            replaceCharWithSpace(temp.place);

            printf("| %-30s | %-20s | %-5.1f | %-5d | %-15lld | %-20s |\n", temp.name, temp.place, temp.rating, temp.price, temp.phoneNumber, temp.mail);
            printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");

            hcount++;
        }
    }
    fclose(fp);
}

void load_lodges(const char key[])
{
    FILE *fp;
    fp = fopen("lodges.txt", "r");

    if (fp == NULL)
    {
        printf("\nCannot open the file");
        return;
    }

    struct lodge temp;

    while (fscanf(fp, "%s %s %d %f %d %lld", temp.name, temp.location, &temp.price, &temp.rating, &temp.stars, &temp.phoneNumber) == 6)
    {
        if (strcmpi(key, temp.location) == 0)
        {
            if (lcount < 100)
            {
                lo[lcount] = temp;
                lcount++;
            }
            else
            {
                printf("\nMaximum number of lodges reached. Cannot read more data.\n");
                break;
            }
        }
    }

    fclose(fp);

    printf("\n-------------------------------------- Lodges in %s -------------------------------------------\n", key);
    printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
    printf("|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n");
    printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");

    for (i = 0; i < lcount; i++)
    {
        replaceCharWithSpace(lo[i].name);
        replaceCharWithSpace(lo[i].location);

        printf("| %-30s | %-20s | %-5d | %-5.1f | %-5d | %-15lld |\n",
               lo[i].name, lo[i].location, lo[i].price, lo[i].rating, lo[i].stars, lo[i].phoneNumber);
        printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
    }
}

void load_restaurants(const char key[])
{
    FILE *fp;
    fp = fopen("restaurants.txt", "r");

    if (fp == NULL)
    {
        printf("\nCannot open the file");
        return;
    }

    struct restaurants temp;
    rcount = 0;

    while (fscanf(fp, "%s %s %s %f %s %lld", temp.name, temp.place, temp.type, &temp.rating, temp.range, &temp.phoneNumber) == 6)
    {
        if (strcmpi(key, temp.place) == 0)
        {
            if (rcount < 100)
            {
                re[rcount] = temp;
                rcount++;
            }
            else
            {
                printf("\n   Maximum number of restaurants reached. Cannot read more data.\n");
                break;
            }
        }
    }

    fclose(fp);

    printf("\n----------------------------------------- Restaurants in %s -------------------------------------\n", key);

    printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
    printf("|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n");
    printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");

    for (i = 0; i < rcount; i++)
    {
        replaceCharWithSpace(re[i].name);
        replaceCharWithSpace(re[i].place);

        printf("| %-30s | %-20s | %-6s | %-5.1f | %-6s | %-15lld |\n",
               re[i].name, re[i].place, re[i].type, re[i].rating, re[i].range, re[i].phoneNumber);
        printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
    }
}

void mergehotel(struct hotel arr[], int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    struct hotel L[n1], R[n2];

    for (i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].rating >= R[j].rating)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergesorthotel(struct hotel arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergesorthotel(arr, left, mid);
        mergesorthotel(arr, mid + 1, right);

        mergehotel(arr, left, mid, right);
    }
}

void filter_hotels()
{
    int choice, min, max, rchoice, pchoice, i;

    while (1)
    {
        printf("\n\n------------------------------");
        printf("\n1. Sort based on price\n2. Sort based on rating\n3. Apply a cost range\n4. Back\n   Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            for (i = 1; i < hcount; ++i)
            {
                struct hotel key = ht[i];
                int j = i - 1;

                while (j >= 0 && ht[j].price > key.price)
                {
                    ht[j + 1] = ht[j];
                    j = j - 1;
                }
                ht[j + 1] = key;
            }

            printf("\n1. High to low\n2. Low to high\n   Enter choice:");
            scanf("%d", &pchoice);
            system("cls");
            switch (pchoice)
            {
            case 1:
                printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                printf("|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n");
                printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");

                for (i = hcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(ht[i].name);
                    printf("| %-30s | %-20s | %-5.1f | %-5d | %-15lld | %-20s |\n", ht[i].name, ht[i].place, ht[i].rating, ht[i].price, ht[i].phoneNumber, ht[i].mail);
                    printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                }
                break;

            case 2:
                printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                printf("|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n");
                printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                for (i = 0; i < hcount; i++)
                {
                    replaceCharWithSpace(ht[i].name);
                    printf("| %-30s | %-20s | %-5.1f | %-5d | %-15lld | %-20s |\n", ht[i].name, ht[i].place, ht[i].rating, ht[i].price, ht[i].phoneNumber, ht[i].mail);
                    printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                }
                break;
            }

            break;

        case 2:
            mergesorthotel(ht, 0, hcount - 1);

            printf("\n1. Best ratings\n2. Least ratings\n   Enter your choice: ");
            scanf("%d", &rchoice);

            switch (rchoice)
            {
            case 2:
                printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                printf("|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n");
                printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                for (i = hcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(ht[i].name);
                    printf("| %-30s | %-20s | %-5.1f | %-5d | %-15lld | %-20s |\n", ht[i].name, ht[i].place, ht[i].rating, ht[i].price, ht[i].phoneNumber, ht[i].mail);
                    printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                }
                break;

            case 1:
                printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                printf("|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n");
                printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                for (i = 0; i < hcount; i++)
                {
                    replaceCharWithSpace(ht[i].name);
                    printf("| %-30s | %-20s | %-5.1f | %-5d | %-15lld | %-20s |\n", ht[i].name, ht[i].place, ht[i].rating, ht[i].price, ht[i].phoneNumber, ht[i].mail);
                    printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                }
                break;
            }
            break;

        case 3:
            printf("\nEnter price range(In rupees):\n");
            printf("Minimum: ");
            scanf("%d", &min);
            printf("Maximum: ");
            scanf("%d", &max);
            printf("\nHotels between price range %d to %d :\n", min, max);
            printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
            printf("|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n");
            printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
            for (i = 0; i < hcount; i++)
            {
                if (ht[i].price >= min && ht[i].price <= max)
                {
                    replaceCharWithSpace(ht[i].name);
                    printf("| %-30s | %-20s | %-5.1f | %-5d | %-15lld | %-20s |\n", ht[i].name, ht[i].place, ht[i].rating, ht[i].price, ht[i].phoneNumber, ht[i].mail);
                    printf("+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n");
                }
            }
            break;

        default:
            return;
        }
    }
}

void mergelo(struct lodge arr[], int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    struct lodge L[n1], R[n2];

    for (i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].rating >= R[j].rating)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergesortlo(struct lodge arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergesortlo(arr, left, mid);
        mergesortlo(arr, mid + 1, right);

        mergelo(arr, left, mid, right);
    }
}

void filter_lodges()
{
    int choice, min, max, rchoice, pchoice, flag, i;
    while (1)
    {
        printf("\n\n------------------------------");
        printf("\n1. Sort based on price\n2. Sort in rating\n3. Apply a cost range\n4. Back\n   Enter choice :");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            for (i = 1; i < lcount; ++i)
            {
                struct lodge key = lo[i];
                int j = i - 1;

                while (j >= 0 && lo[j].price > key.price)
                {
                    lo[j + 1] = lo[j];
                    j = j - 1;
                }
                lo[j + 1] = key;
            }

            printf("\n1. High to low\n2. Low to high\n   Enter your choice:");
            scanf("%d", &pchoice);

            switch (pchoice)
            {
            case 1:
                printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                printf("|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n");
                printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                for (i = lcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(lo[i].name);
                    printf("| %-30s | %-20s | %-5d | %-5.1f | %-5d | %-15lld |\n",
                           lo[i].name, lo[i].location, lo[i].price, lo[i].rating, lo[i].stars, lo[i].phoneNumber);
                    printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                }
                break;

            case 2:
                printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                printf("|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n");
                printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                for (i = 0; i < lcount; i++)
                {
                    replaceCharWithSpace(lo[i].name);
                    printf("| %-30s | %-20s | %-5d | %-5.1f | %-5d | %-15lld |\n",
                           lo[i].name, lo[i].location, lo[i].price, lo[i].rating, lo[i].stars, lo[i].phoneNumber);
                    printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                }
                break;
            }
            break;

        case 2:
            mergesortlo(lo, 0, lcount - 1);

            printf("\n1. Best ratings\n2. Least ratings\n   Enter your choice: ");
            scanf("%d", &rchoice);

            switch (rchoice)
            {
            case 1:
                printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                printf("|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n");
                printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                for (i = 0; i < lcount; i++)
                {
                    replaceCharWithSpace(lo[i].name);
                    printf("| %-30s | %-20s | %-5d | %-5.1f | %-5d | %-15lld |\n",
                           lo[i].name, lo[i].location, lo[i].price, lo[i].rating, lo[i].stars, lo[i].phoneNumber);
                    printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                }
                break;

            case 2:
                printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                printf("|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n");
                printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                for (i = lcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(lo[i].name);
                    printf("| %-30s | %-20s | %-5d | %-5.1f | %-5d | %-15lld |\n",
                           lo[i].name, lo[i].location, lo[i].price, lo[i].rating, lo[i].stars, lo[i].phoneNumber);
                    printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                }
                break;
            }
            break;

        case 3:
            printf("\nEnter price range(In rupees):\n");
            printf("Minimum: ");
            scanf("%d", &min);
            printf("Maximum: ");
            scanf("%d", &max);
            printf("\n------------Lodges between price range %d to %d------------\n", min, max);

            flag = 0;
            printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
            printf("|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n");
            printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
            for (i = 0; i < lcount; i++)
            {
                if (lo[i].price >= min && lo[i].price <= max)
                {
                    replaceCharWithSpace(lo[i].name);
                    printf("| %-30s | %-20s | %-5d | %-5.1f | %-5d | %-15lld |\n",
                           lo[i].name, lo[i].location, lo[i].price, lo[i].rating, lo[i].stars, lo[i].phoneNumber);
                    printf("+-------------------------------+----------------------+-------+-------+-------+-----------------+\n");
                    flag = 1;
                }
            }
            if (flag == 0)
            {
                printf("\nNo restaurants found");
            }
            break;

        default:
            return;
        }
    }
}

void mergert(struct restaurants arr[], int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    struct restaurants L[n1], R[n2];

    for (i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].rating <= R[j].rating)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergesortrt(struct restaurants arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergesortrt(arr, left, mid);
        mergesortrt(arr, mid + 1, right);

        mergert(arr, left, mid, right);
    }
}

void filter_restaurants()
{
    int choice, rchoice, flag, i;
    char filterType[10], filterprice[10];

    while (1)
    {
        printf("\n\n------------------------");
        printf("\n1. Sort based on rating\n2. Sort on Veg/NonVeg/Both\n3. Sort based on price\n4. Back\n   Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            mergesortrt(re, 0, rcount - 1);

            printf("\n1. Best ratings\n2. Least ratings\n   Enter your choice: ");
            scanf("%d", &rchoice);

            switch (rchoice)
            {
            case 1:
                printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
                printf("|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n");
                printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
                for (i = rcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(re[i].name);
                    printf("| %-30s | %-20s | %-6s | %-5.1f | %-6s | %-15lld |\n",
                           re[i].name, re[i].place, re[i].type, re[i].rating, re[i].range, re[i].phoneNumber);
                    printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
                }
                break;

            case 2:
                printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
                printf("|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n");
                printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");

                for (i = 0; i < rcount; i++)
                {
                    replaceCharWithSpace(re[i].name);
                    printf("| %-30s | %-20s | %-6s | %-5.1f | %-6s | %-15lld |\n",
                           re[i].name, re[i].place, re[i].type, re[i].rating, re[i].range, re[i].phoneNumber);
                    printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
                }
                break;
            }
            break;

        case 2:
            printf("\n   Enter filter type (Veg/Non-Veg/Both): ");
            scanf("%s", filterType);

            printf("\n----------------filtered list----------------\n");

            flag = 0;
            printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
            printf("|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n");
            printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");

            for (i = 0; i < rcount; i++)
            {
                if (strcasecmp(filterType, re[i].type) == 0)
                {
                    replaceCharWithSpace(re[i].name);
                    printf("| %-30s | %-20s | %-6s | %-5.1f | %-6s | %-15lld |\n",
                           re[i].name, re[i].place, re[i].type, re[i].rating, re[i].range, re[i].phoneNumber);
                    printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
                    flag = 1;
                }
            }

            if (flag == 0)
            {
                printf("\nNo restaurants found");
            }
            break;

        case 3:
            printf("\n   Enter price type (high/average/low): ");
            scanf("%s", filterprice);

            printf("\n----------------filtered list----------------\n");
            printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
            printf("|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n");
            printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");

            flag = 0;
            for (i = 0; i < rcount; i++)
            {
                if (strcasecmp(filterprice, re[i].range) == 0)
                {
                    replaceCharWithSpace(re[i].name);
                    printf("| %-30s | %-20s | %-6s | %-5.1f | %-6s | %-15lld |\n",
                           re[i].name, re[i].place, re[i].type, re[i].rating, re[i].range, re[i].phoneNumber);
                    printf("+-------------------------------+----------------------+--------+-------+--------+-----------------+\n");
                    flag = 1;
                }
            }
            if (flag == 0)
            {
                printf("\nNo restaurants found");
            }
            break;

        default:
            return;
        }
    }
}

void info()
{
    int choice, back = 0, temp;
    char loc[50];

retake:
    printf("\n   Enter your current location : ");
    scanf("%s", loc);

    if (checklist(loc) == 1000)
    {
        goto retake;
    }
opt:
    while (1)
    {
        // printf("\n------------------------ ");
        printf("\n1. Hotels in %s", loc);
        printf("\n2. Lodges in %s", loc);
        printf("\n3. Restaurants in %s", loc);
        printf("\n4. Go back to home page");
        printf("\n5. Quit app");
        printf("\n   Enter your choice : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            hcount = 0;
            load_hotels(loc);

            printf("\n\n1. Go back\n2. Apply filter");
            printf("\n   Enter your choice:");
            scanf("%d", &temp);

            switch (temp)
            {
            case 1:
                system("cls");
                goto opt;
                break;

            case 2:
                filter_hotels();
                system("cls");
                break;
            }
            break;

        case 2:
            lcount = 0;
            load_lodges(loc);

            printf("\n\n1.Go back\n2.Apply filter");
            printf("\n  Enter your choice:");
            scanf("%d", &temp);

            switch (temp)
            {
            case 1:
                system("cls");
                goto opt;
                break;

            case 2:
                filter_lodges();
                system("cls");
                break;
            }
            break;

        case 3:
            rcount = 0;
            load_restaurants(loc);

            printf("\n\n1.Go back\n2.Apply filter");
            printf("\n  Enter your choice:");
            scanf("%d", &temp);

            switch (temp)
            {
            case 1:
                system("cls");
                goto opt;
                break;
            case 2:
                filter_restaurants();
                system("cls");
                break;
            }
            break;

        case 4:
            back = 1;
            break;

        case 5:
            exit(0);

        default:
            printf("Invalid input");
        }
        if (back == 1)
        {
            break;
        }
    }
}

int checkph(long long int *phoneno)
{
    int count = 0;
    long long int temp = (*phoneno);
    while (temp != 0)
    {
        count++;
        temp = temp / 10;
    }
    return count;
}

void enlisth()
{
    FILE *fp;
    struct hotel newHotel;

    fp = fopen("hotels.txt", "a");
    if (fp == NULL)
    {
        printf("\nError opening the file.");
        return;
    }

    printf("\n-----------------------------");
    printf("\nEnter Details of Hotel to enlist\n");
    printf("Name: ");
    scanf("%s", newHotel.name);

    printf("Location: ");
    scanf("%s", newHotel.place);

    printf("Rating: ");
    scanf("%f", &newHotel.rating);

    printf("Price: ");
    scanf("%d", &newHotel.price);

Enter_phoneno:
    printf("Phone number: ");
    scanf("%lld", &newHotel.phoneNumber);
    if (checkph(&newHotel.phoneNumber) != 10)
    {
        printf("\nNot a valid phone number.\n");
        goto Enter_phoneno;
    }

Enter_email:
    printf("Email: ");
    scanf("%s", newHotel.mail);
    if (strncmp(strrev(newHotel.mail), "moc.liamg@", 10))
    {
        printf("Not a valid gmail id.\n");
        goto Enter_email;
    }

    fprintf(fp, "\n%s %s %.1f %d %lld %s", newHotel.name, newHotel.place, newHotel.rating, newHotel.price, newHotel.phoneNumber, strrev(newHotel.mail));

    printf("Hotel information added successfully!\n");

    fclose(fp);
    return;
}

void enlistl()
{
    FILE *fp;
    struct lodge newLodge;

    fp = fopen("lodges.txt", "a");
    if (fp == NULL)
    {
        printf("\nError opening the file.");
        return;
    }

    printf("\n-----------------------------");
    printf("\nEnter Details of Lodge to enlist\n");
    printf("Name: ");
    scanf("%s", newLodge.name);

    printf("Location: ");
    scanf("%s", newLodge.location);

    printf("Price: ");
    scanf("%d", &newLodge.price);

    printf("Rating: ");
    scanf("%f", &newLodge.rating);

    printf("Stars: ");
    scanf("%d", &newLodge.stars);

Enter_phoneno:
    printf("Phone number: ");
    scanf("%lld", &newLodge.phoneNumber);
    if (checkph(&newLodge.phoneNumber) != 10)
    {
        printf("\nNot a valid phone number.\n");
        goto Enter_phoneno;
    }

    fprintf(fp, "\n%s %s %d %.1f %d %lld", newLodge.name, newLodge.location, newLodge.price, newLodge.rating, newLodge.stars, newLodge.phoneNumber);

    printf("Lodge information added successfully!\n");

    fclose(fp);
}

void enlistr()
{
    FILE *fp;
    struct restaurants newRestaurant;

    fp = fopen("restaurants.txt", "a");

    if (fp == NULL)
    {
        printf("\nError opening the file.");
        return;
    }

    printf("\n-----------------------------");
    printf("\nEnter Details of Restaurant to enlist\n");
    printf("Name: ");
    scanf("%s", newRestaurant.name);

    printf("Location: ");
    scanf("%s", newRestaurant.place);

    printf("Type: ");
    scanf("%s", newRestaurant.type);

    printf("Rating: ");
    scanf("%f", &newRestaurant.rating);

    printf("Price range: ");
    scanf("%s", newRestaurant.range);

Enter_phoneno:
    printf("Phone number: ");
    scanf("%lld", &newRestaurant.phoneNumber);
    if (checkph(&newRestaurant.phoneNumber) != 10)
    {
        printf("\nNot a valid phone number.\n");
        goto Enter_phoneno;
    }

    fprintf(fp, "%s %s %s %.1f %s %lld\n", newRestaurant.name, newRestaurant.place, newRestaurant.type, newRestaurant.rating, newRestaurant.range, newRestaurant.phoneNumber);

    printf("Restaurant information added successfully!\n");

    fclose(fp);
}

int loginUser(char username[], char password[])
{
    FILE *fp;
    User user;

    fp = fopen("user_credentials.txt", "r");
    if (fp == NULL)
    {
        printf("\nError opening user credentials file.");
        return 0;
    }

    while (fscanf(fp, "%s %s", user.username, user.password) == 2)
    {
        if (strcmp(username, user.username) == 0 && strcmp(password, user.password) == 0)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void createUser()
{
    FILE *fp;
    char uname[20], pass[20];
    User newUser;

    fp = fopen("user_credentials.txt", "r");
    if (fp == NULL)
    {
        printf("\nError opening file.");
        return;
    }

tryagain:
    printf("\n   Enter a new username: ");
    scanf("%s", newUser.username);

    while (fscanf(fp, "%s %s\n", uname, pass) == 2)
    {
        if (stricmp(newUser.username, uname) == 0)
        {
            printf("\n   Username already exits, Try %s1\n", newUser.username);
            fseek(fp, 0, SEEK_SET);
            goto tryagain;
        }
    }
    printf("   Enter a password: ");
    scanf("%s", newUser.password);
    fclose(fp);

    fp = fopen("user_credentials.txt", "a");
    if (fp == NULL)
    {
        printf("\nError opening file.");
        return;
    }

    fprintf(fp, "%s %s\n", newUser.username, newUser.password);
    printf("\n   User account created successfully!\n");

    fclose(fp);
}

void loadUserPreferences(char username[])
{
    FILE *fp;
    User user;
    int flag = 0;

    fp = fopen("user_preferences.txt", "r");
    if (fp == NULL)
    {
        printf("\nError opening file.");
        return;
    }

    while (fscanf(fp, "%s %s %s %s\n", user.username, user.fav_location, user.fav_restaurant, user.fav_lodge) == 4)
    {
        if (strcmp(username, user.username) == 0)
        {
            flag = 1;
            replaceCharWithSpace(user.fav_restaurant);
            replaceCharWithSpace(user.fav_lodge);
            printf("\n   Favourite location:   %s\n   Favourite restaurant: %s\n   Favourite Lodge:      %s\n", user.fav_location, user.fav_restaurant, user.fav_lodge);
        }
    }

    if (flag == 0)
    {
        printf("\n   No favourites\n");
    }

    fclose(fp);
    return;
}

void saveUserPreferences(char username[])
{
    FILE *fp;
    char loc[50], rest[50], lod[50];

    fp = fopen("user_preferences.txt", "a");
    if (fp == NULL)
    {
        printf("\nError opening file.");
        return;
    }

    printf("\n   Enter your Favourite\n   Location:");
    scanf("%s", loc);
    printf("   Restaurant:");
    scanf("%s", rest);
    printf("   Lodge:");
    scanf("%s", lod);

    fprintf(fp, "%s %s %s %s\n", username, loc, rest, lod);

    printf("\n   User preferences saved successfully!\n");

    fclose(fp);
}

void addfeedback(char usernamr[])
{
    int choice;
    char name[20], feedbackDesc[100];

    FILE *fp = fopen("feedbacks.txt", "a");

    if (fp == NULL)
    {
        printf("\nError opening file.");
        return;
    }

here:
    printf("\n   feedback on:\n1. Hotel\n2. Lodge\n3. Restaurant\n   Enter choice:");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("\n   Enter Hotel name:");
        scanf("%s", name);
        printf("\n   Write your feedback:");
        scanf("%s", feedbackDesc);

        fprintf(fp, "\n%c %s %s\n", 'h', name, feedbackDesc);
        break;
    case 2:
        printf("\n   Enter Lodge name:");
        scanf("%s", name);
        printf("\n   Write your feedback:");
        scanf("%s", feedbackDesc);

        fprintf(fp, "\n%c %s %s\n", 'l', name, feedbackDesc);
        break;
    case 3:
        printf("\n   Enter Restaurant name:");
        scanf("%s", name);
        printf("   Write your feedback:");
        scanf("%s", feedbackDesc);

        fprintf(fp, "\n%c %s %s\n", 'r', name, feedbackDesc);
        break;
    default:
        goto here;
    }
    printf("\n   feedback uploaded.\n");
    fclose(fp);
    return;
}

void loadfeedbacks(char username[])
{
    int choice, flag;
    char code[1], name[20], feedbackDesc[200];
    FILE *fp;
    fp = fopen("reviews.txt", "r");

    if (fp == NULL)
    {
        printf("\nCannot Access The Data-Base");
    }

here:
    printf("\n Check your feedbacks \n");
    printf("1. Hotel\n2. Lodge\n3. Restaurant\n4. Back\n   Enter choice:");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        flag = 0;

        fp = fopen("reviews.txt", "r");

        while (fscanf(fp, "%s %s %s\n", code, name, feedbackDesc) == 3)
        {
            if (strcmp(code, "h") == 0)
            {
                replaceCharWithSpace(name);
                replaceCharWithSpace(feedbackDesc);
                printf("\n   Hotel Name: %s\tfeedback: %s\n", name, feedbackDesc);
                flag = 1;
            }
        }

        if (flag == 0)
        {
            printf("\n   No feedbacks on hotels\n");
        }

        fseek(fp, 0, SEEK_SET);
        
        goto here;
        break;

    case 2:
        flag = 0;

        while (fscanf(fp, "%s %s %s\n", code, name, feedbackDesc) == 3)
        {
            if (strcmp(code, "l") == 0)
            {
                replaceCharWithSpace(name);
                replaceCharWithSpace(feedbackDesc);
                printf("\n   Lodge Name: %s\tfeedback: %s\n", name, feedbackDesc);
                flag = 1;
            }
        }

        if (flag == 0)
        {
            printf("\n   No feedbacks on lodges\n");
        }

        fseek(fp, 0, SEEK_SET);

        goto here;
        break;

    case 3:
        flag = 0;

        while (fscanf(fp, "%s %s %s\n", code, name, feedbackDesc) == 3)
        {
            if (strcmp(code, "r") == 0)
            {
                replaceCharWithSpace(name);
                replaceCharWithSpace(feedbackDesc);
                printf("\n   Restaurant Name: %s\tfeedback: %s\n", name, feedbackDesc);
                flag = 1;
            }
        }

        if (flag == 0)
        {
            printf("\n   No feedbacks on restaurant\n");
        }
        fseek(fp, 0, SEEK_SET);

        goto here;
        break;

    case 4:
        fclose(fp);
        return;

    default:
        goto here;
    }
    
}

void getPassword(char *password, int max_length)
{
    int i = 0;
    char ch;

    while (1)
    {
        ch = getch();
        if (ch == 13 || ch == 10)
        {
            password[i] = '\0';
            break;
        }
        else if (ch == 8 && i > 0)
        {
            i--;
            printf("\b \b");
        }
        else if (i < max_length - 1)
        {
            password[i] = ch;
            i++;
            printf("*");
        }
    }
    printf("\n");
}

void logLogin(const char *username)
{
    FILE *file = fopen("logins.txt", "a"); // Open file in append mode

    if (file != NULL)
    {
        time_t t;
        time(&t);
        char *loginTime = ctime(&t);
        loginTime[strlen(loginTime) - 1] = '\0';

        fprintf(file, "Username: %s\n", username);
        fprintf(file, "Login Time: %s\n", loginTime);
        fprintf(file, "-----------------\n");

        fclose(file);
        printf("\n   Login successful!\n");
        printf("   Logged-in at (date and time): %s\n", loginTime);
    }
    else
    {
        printf("\n   Error opening log file.\n");
    }
}

int main()
{
    int choice, achoice, uchoice, rchoice, maxLength = 20;
    char username[50], password[50];

    load_adjacency();
    load_distance();
    load_cities();
redo:
    printf("\n-----------------------------------------------------------------------------------\n");
    printf("                                         Welcome                                     \n");
    printf("-----------------------------------------------------------------------------------\n");

    printf("\n");
    printf("    Login-Page \n");
    printf(".....................\n");
user1:
    printf("1. Login\n");
    printf("2. Create Account\n");
    printf("Enter choice: ");
    scanf("%d", &achoice);
    system("cls");
    switch (achoice)
    {
    case 1:
        printf("\n-------------------------\n");
        printf("Login\n");
        printf("-------------------------\n");

        printf("Enter your username: ");
        scanf("%s", username);

        printf("Enter your password: ");
        getPassword(password, maxLength);
        system("cls");
        printf("\n-------------------------\n");

        if (loginUser(username, password))
        {
            printf("Login successful! Welcome back, %s!\n", username);
            logLogin(username);
        }
        else
        {
            printf("Invalid username or password. Please try again.\n");
            goto user1;
        }
        break;

    case 2:
        printf("\n-------------------------\n");
        printf("Create Account\n");
        printf("-------------------------\n");

        createUser();
        system("cls");
        printf("Account created successfully!\n");
        break;

    default:
        printf("Invalid choice. Please enter 1 or 2.\n");
        goto user1;
    }

    while (1)
    {
        printf("\n============================================================================================\n");
        printf("                                 Welcome to Mumbai-Residences           \n");
        printf("============================================================================================\n");

        printf("1. Travel from Karnataka to Mumbai\n");

        printf("2. Search for hotels, lodges, and restaurants in Mumbai\n");

        printf("3. Enlist new hotels, lodges, or restaurants in the database\n");

        printf("4. User library\n");

        printf("5. Quit\n");

        printf("============================================================================================\n");

        printf(" Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            system("cls");
            KTM();
            break;

        case 2:
            system("cls");
            info();
            break;

        case 3:
            system("cls");
            printf("============================================================================================\n");

            printf("1. Enlist hotel\n");
            printf("2. Enlist lodge\n");
            printf("3. Enlist restaurant\n");

            printf("   Enter choice:");
            scanf("%d", &choice);
            printf("============================================================================================\n");

            if (choice == 1)
            {
                enlisth();
            }
            else if (choice == 2)
            {
                enlistl();
            }
            else
            {
                enlistr();
            }
            break;

        case 4:
        user:
            printf("============================================================================================\n");
            printf("                          User Library             \n");
            printf("============================================================================================\n");

            printf(" 1. My Favorites\n");
            printf(" 2. Add to My Favorites\n");
            printf(" 3. Feedback for Hotel, Lodge, or Restaurant\n");
            printf(" 4. Back\n");

            printf(" Enter choice: ");
            scanf("%d", &uchoice);

            switch (uchoice)
            {
            case 1:
                loadUserPreferences(username);
                goto user;
                break;

            case 2:
                saveUserPreferences(username);
                goto user;
                break;
            case 3:
            feedback:

                printf("\n===========================================\n");
                printf("         Feedback Options Menu              \n");
                printf("===========================================\n");

                printf(" 1. Add My Feedback\n");
                printf(" 2. See All Feedbacks\n");
                printf(" 3. Back\n");

                printf(" Enter choice: ");
                scanf("%d", &rchoice);

                switch (rchoice)
                {
                case 1:
                    addfeedback(username);
                    goto user;
                    break;

                case 2:
                    loadfeedbacks(username);
                    goto user;
                    break;

                case 3:
                    goto user;

                default:
                    goto feedback;
                }
                break;
            }

            break;

        case 5:
            printf("=============================================\n");
            printf("              Thank You!\n");
            printf("=============================================\n\n");
            exit(0);

        default:
            printf("\nInvalid input....");
        }
    }
}