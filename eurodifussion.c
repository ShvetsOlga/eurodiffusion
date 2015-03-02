#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define COIN 1000000
#define COIN_PER_TIME 1000
#define CITY_LENGTH 25
#define MAX_COUNTRY 20
#define MAX_STRING 80
/*
 * Declaration block
 *
 */
/*
 * Structure for country
 */
typedef struct
  {
    char name[CITY_LENGTH];
    int num_cities;
    int border_xl;
    int border_xh;
    int border_yl;
    int border_yh;
    int complete_country;
    int flag;
    int step;
  } COUNTRY;
/*
 * Structure for city
 */

  typedef struct
  {
      int city_number;
      int country_number;
      int* balance;
      int* debit;
      int* credit;
      int complete_city;
  } CITY;

int country_vol;// Quantity of country
int cities_vol;// Quantity of city
int min_max[4];//Array for save  borders of Europe
int** temp_array;
int** matrix;//Adjacency matrix  
COUNTRY* country_list;
CITY* city_list;

typedef struct
{
    int country_vol;
    COUNTRY* countries;
}TASK;


TASK* task_list;
/*
 * Declaration of functions
 *
 */

/*
 *Functions of compare for function qsort
 */
int cmpint(const void* p1, const void* p2)
{
    return ((COUNTRY)(*((COUNTRY*)p1))).flag - ((COUNTRY)(*((COUNTRY*)p2))).flag;
}

int cmpstr(const void* p1, const void* p2)
{
    return strcmp(((COUNTRY)(*((COUNTRY*)p1))).name, ((COUNTRY)(*((COUNTRY*)p2))).name);
}
/*
 * Fuction receive quantity of countries and return array
 * COUNTRY type
 *
 */

COUNTRY* country_array(int x)
{
    COUNTRY* t;
    t = (COUNTRY*) malloc(sizeof(COUNTRY) * x);
    if (t == NULL)
    {
        printf("Can't allocate enough memory\n");
        exit(EXIT_FAILURE);
    }
    return t;
}
/*
 * Function receive quantity of cities as first parametr
 * and quantity of countries as second parametr
 * and return array CITY type
 */
CITY* city_array(int x, int y)
{
    CITY* t;
    t = (CITY*) malloc(sizeof(CITY) * x);
    if (t == NULL)
    {
        printf("Can't allocate enough memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < x; i++)
    {
      t[i].balance = (int*) malloc(sizeof(int) * y);
      t[i].debit = (int*) malloc(sizeof(int) * y);
      t[i].credit = (int*) malloc(sizeof(int) * y);
    }
    return t;
}
/*
 * Function receive quantity of tasks from input file and
 * return array TASK type
 */

TASK* create_task_array(int x)
{
    TASK* t;
    t = (TASK*) malloc(sizeof(TASK) * x);
    if (t == NULL)
    {
        printf("Can't allocate enough memory\n");
        exit(EXIT_FAILURE);
    }
    return t;
}
/*
 * Function receive quantity of strings and columns
 * and return two dimension array INTEGER type
 */
int** create_2dim_array(int str, int clm)
{
  int** t;  
  t = (int**) malloc(sizeof(int*) * str);
  if (t == NULL)
  {
    printf("Can't allocate enough memory\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < str; i++)
  {
      t[i] = (int*) malloc(sizeof(int) * clm);
        if (t[i] == NULL)
        {
            printf("Can't allocate enough memory\n");
            exit(EXIT_FAILURE);
        }
  }
  return t;
}
/*
 * Function receive two dimension array, quantity of
 * strings and columns in this array and integer number
 * for filling array
 */
void infill_2dim_array(int** a, int str, int clm, int infill)
{
    for (int i = 0; i < str; i++)
    {
        for (int j = 0; j < clm; j++)
            a[i][j] = infill;
    }
}
/*
 * Function receive array min_max and array COUNTRY type
 * and return minimum and maximum of cities coordinates
 * as min_max array
 */
int extremum(int* x, COUNTRY* a)
{
    int min_x = 1;
    int max_x = 0;
    int min_y = 1;
    int max_y = 0;
    for (int i = 0; i < country_vol; i++)
    {
        min_x = a[i].border_xl < min_x ? a[i].border_xl : min_x;
        min_y = a[i].border_yl < min_y ? a[i].border_yl : min_y;
        max_x = a[i].border_xh > max_x ? a[i].border_xh : max_x;
        max_y = a[i].border_yh > max_y ? a[i].border_yh : max_y;
    } 
    x[0] = min_x;
    x[1] = min_y;
    x[2] = max_x;
    x[3] = max_y;
    return 0;
}

/*
 * Function receive array COUNTRY type and infill array
 * values of quantity cities in each country and 
 * return quantity all cities in task
 */

int cities_in_country(COUNTRY* x)
{
    int k = 0;
    for (int i = 0; i < country_vol; i++)
    {
        x[i].num_cities = (x[i].border_xh - x[i].border_xl + 1) * (x[i].border_yh - x[i].border_yl + 1);
        k += x[i].num_cities;
    }
    return k;
}

/*
 * Function receive two dimension array with "-1" in each
 * volumes, array COUNTRY type and min_max array and
 * infill two dimension array numbers of all cities
 * in TASK from zero to quantity of countries.
 * In fact we get map of Europe for each task.
 */

void infill_temp_array(int** t, COUNTRY* a, int* m)
{
    int  k, start_y, end_y;
    for (int i = 0; i < country_vol; i++)
    {
        start_y = m[3] - a[i].border_yh;
        end_y = m[3] - a[i].border_yl;
        for (int j = start_y ; j <= end_y; j++)
        {
            for (k = a[i].border_xl - 1; k <= a[i].border_xh - 1; k++)
                t[j][k] = i;
        }
    }
}
/*
 * Function receive array CITY type and infill it cities from
 * array with numbers of cities. Number of each city
 * equal index of array
 */
void infill_city_list(CITY* t)
{
  int k = 0;
  for(int i = 0; i < min_max[3]; i++)
  {
      for (int j = 0; j < min_max[2]; j++)
      {
           if (temp_array[i][j] != -1) 
           {
               city_list[k].country_number = temp_array[i][j];
               temp_array[i][j] = k;
               city_list[k].city_number = k;
               k++;
           }
      }
  }
}

/*
 * Function infill adjacency matrix
 */
void infill_adjacency_matrix(int** t)
{
  for (int i = 0; i < min_max[3]; i++)
  {
      for (int j = 0; j < min_max[2] - 1; j++)
      {
          if ((temp_array[i][j] != -1) && (temp_array[i][j+1] != -1))
              {
                  t[temp_array[i][j]][temp_array[i][j+1]]  = 1;
                  t[temp_array[i][j+1]][temp_array[i][j]]  = 1;
              }
      }
  }
  for (int i = 0; i < min_max[2]; i++)
  {
      for (int j = 0; j < min_max[3] - 1; j++)
      {
          if ((temp_array[j][i] != -1) && (temp_array[j+1][i] != -1))
              {
                  t[temp_array[j][i]][temp_array[j+1][i]]  = 1;
                  t[temp_array[j+1][i]][temp_array[j][i]]  = 1;
              }
      }
  }
}

/*
 * Initialization array of cities
 */
void init_city_array(CITY* t)
{
  for (int i = 0; i < cities_vol; i++)
  {
    for (int j = 0; j < country_vol; j++)
    {
        t[i].debit[j] = 0;
        t[i].credit[j] = 0;
        if(j == t[i].country_number)
            t[i].balance[j] = COIN;
        else 
            t[i].balance[j] = 0;
    }
    t[i].complete_city = 0;
  }
}

/*
 * Initialization array of countries
 */
void init_country_array(COUNTRY* t)
{
  for (int i = 0; i < country_vol; i++)
  {
      t[i].complete_country = t[i].num_cities;
      t[i].step = 1;
  }
}

/*
 * Calculate all days for full distribution eurocoins
 */

void diffusion(CITY* city, COUNTRY* country)
{
  int check = country_vol;
  int step = 0;
  for (step = 0; check > 0; step++)
  {
      for (int i = 0; i < cities_vol; i++)
      {
          int z = 0;
          for (int j = 0; j < country_vol; j++)
          {
              city[i].balance[j] += city[i].debit[j];
              city[i].debit[j] = 0;
              city[i].credit[j] = city[i].balance[j] / 1000;
              if (city[i].complete_city == 0)
              {
              if (city[i].balance[j])
                  z++;
              }
              
          }
          
          if (z == country_vol)
          {
              city[i].complete_city = 1;
              country[city[i].country_number].complete_country--;
              
          }
      }    
/*
 * Check country complete
 *
 */
check = country_vol;
for(int i = 0; i < country_vol; i++)
{
    if(country[i].complete_country == 0)
    {
        check--;
        continue;
    }
    else
        country[i].flag = step + 1;
}

/*
 * Exchange coins is there
 *
 */
      for (int i = 0; i < cities_vol; i++)
      {
          for (int j = 0; j <= i; j++)
          {
              if (matrix[i][j])
              {
                  for (int k = 0; k < country_vol; k++)
                  {
                      city[i].debit[k] += city[j].credit[k];
                      city[j].debit[k] += city[i].credit[k];
                      city[i].balance[k] -= city[i].credit[k];
                      city[j].balance[k] -= city[j].credit[k];
                  }
              
              }
          }
      }
  }
}

/*
 * Start function main
 */

int main(int argc, char** argv)
{
    FILE* input;
    if (argc < 2)
    {
        printf("Please use %s as %s inputfile_name\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }
    input = fopen(argv[1],"r");
    if (input == NULL)
    {
        printf("Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    char** BUFF = (char**) malloc(sizeof(char*) * MAX_COUNTRY);
    for (int i = 0; i < MAX_COUNTRY; i++)
        BUFF[i] = (char*) malloc(sizeof(char) * MAX_STRING);
    int i = 0;
    int j = 0;
    while (fgets(BUFF[i], MAX_STRING, input)) i++;
    fclose(input);
    i = 0;
    int count_task = 0;
    while (BUFF[i][0] != '0')
    {
        if (isdigit(BUFF[i][0]))
           count_task++;
           i++;
    }
    task_list = create_task_array(count_task);
    i = 0;
    j = 0;
    while (BUFF[j][0] != '0')
    {
        if (isdigit(BUFF[j][0]))
        {
        task_list[i].country_vol = atoi(BUFF[j]);
        i++;
        }
        j++;
    }

    int* temp = (int*) malloc(sizeof(int) * 6);
    for (int i = 0; i < count_task; i++)
      task_list[i].countries = country_array(task_list[i].country_vol); 
   
  char* token;
  char* delim = " ";
  char* str1;
  int q = 0;
  for (int k = 0; k < count_task; k++)
  {
  for (i = 0; i < task_list[k].country_vol; i++)
  {
      for (j = 1, str1 = BUFF[i + 1 + q]; ; j++, str1 = NULL)
      {
          token = strtok(str1, delim);
          if (token == NULL)
              break;
          if (isalpha(token[0]))
          {
              strcpy(task_list[k].countries[i].name, token);
              continue;
          }
          else
              temp[j] = atoi(token);
          
      }
          task_list[k].countries[i].border_xl = temp[2];
          task_list[k].countries[i].border_xh = temp[4];
          task_list[k].countries[i].border_yl = temp[3];
          task_list[k].countries[i].border_yh = temp[5];

  }
  q += (task_list[k].country_vol + 1);
  }
  free(temp);
  i=0;
  for (i = 0; i < MAX_COUNTRY; i++)
    free(BUFF[i]);
  free(BUFF);
  printf("Sample Input\n\n");
  for (i = 0; i < count_task; i++)
  {
      printf("%-2d\n", task_list[i].country_vol);
      for (j = 0; j < task_list[i].country_vol; j++)
          printf("%-25s%2d%2d%2d%2d\n", task_list[i].countries[j].name, task_list[i].countries[j].border_xl, task_list[i].countries[j].border_yl
          , task_list[i].countries[j].border_xh, task_list[i].countries[j].border_yh);
  }
  printf("%-2d\n",0); 
  printf("\n\n");
  printf("Sample Output\n\n");


  for (i = 0; i < count_task; i++)
  {
    country_vol = task_list[i].country_vol;
    extremum(min_max, task_list[i].countries);
    cities_vol = cities_in_country(task_list[i].countries);
    temp_array = create_2dim_array(min_max[3], min_max[2]);
    infill_2dim_array(temp_array, min_max[3], min_max[2], -1);
    infill_temp_array(temp_array, task_list[i].countries, min_max);
    city_list = city_array(cities_vol, country_vol);
    infill_city_list(city_list);
    matrix = create_2dim_array(cities_vol, cities_vol);
    infill_2dim_array(matrix, cities_vol, cities_vol, 0);
    infill_adjacency_matrix(matrix);
    init_city_array(city_list);
    init_country_array(task_list[i].countries);
    diffusion(city_list, task_list[i].countries);
    qsort(task_list[i].countries, task_list[i].country_vol, sizeof(COUNTRY), cmpint);
    for (j = 0; j <= task_list[i].country_vol - 1; j++)
    {
        if (task_list[i].countries[j].flag == task_list[i].countries[j+1].flag)
        {
            int s = j;
            int c = 1;
        do
        {
            c++;
            s++;
        } while (s < task_list[i].country_vol && task_list[i].countries[s].flag == task_list[i].countries[s+1].flag);
            qsort(task_list[i].countries + j, c, sizeof(COUNTRY), cmpstr);
            j = s;
        }
    }


    printf("Case Number %d\n", i+1); 
    for (int j= 0; j < country_vol; j++)
    {
            printf("%-25s%5d\n", task_list[i].countries[j].name, task_list[i].countries[j].flag );
    }
    printf("\n");
  }  
    return 0;
    
}
