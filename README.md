## About

Serialize C structs into csv files by providing only number of rows, field conversion functions and field sizes.

## Why

I have often found serialization / deserialization as one of most frequent tasks in software. While one can achieve the same by writing normal C code, it becomes a mundane and error prone task when you need to do it again for some other module / structure.

Also, by this project I want to highlight the separation of **user** definitions from the library definitions. The library (`core.c`) has no idea how the structures are implemented. It only knows how to retrieve them (`void*`) using function pointers passed in its config from the user layer. This **separation of control** is often desirable in large projects and also demonstrates good software practice of loosely coupled modules, while maintaining high cohesion inside a module.

While it may seem an over-work in this repo, but, if you follow the examples, you can add your own structure for serialization easily too. I have also provided *person* `struct` example which can use both global memory pool and heap storage for an array of people.

## Compile and run

```bash
make
./main.out
```
## Output


```c
struct Address_Internal_Rep {
    int flat_no;
    COUNTRY country;
    char city[20];
};

struct Person_Internal_Rep {
    char name[NAME_BUF_SIZE];
    char pronouns[PRONOUNS_BUF_SIZE];
    uint8_t age;
    Address_Internal_Rep address;
    Person_Internal_Rep* next;
};
```

gets converted into -> 


```csv
prabal, he/him, 25, flat_no. 204, Noida, IND, 
gopal, he/him, 23, flat_no. 101, Delhi, IND, 
shin dongjoon, he/him, 29, flat_no. 305, Gyeonggi-do, KOR, 
woo soyeon, she/her, 26, flat_no. 204, Busan, KOR,
```

## Files

```bash
devp@IdeaPad:/mnt/d/csv_github/csv$ tree -L 2
.
├── Makefile
├── README.md
├── adapter.h
├── core.c
├── core.h
├── examples
│   ├── external_definitions_ehs_schd.c
│   ├── external_definitions_ehs_schd.h
│   ├── external_definitions_person.c
│   └── external_definitions_person.h
├── main.c
```

Esentially, there are two major code files -  

- `core.c` (converts your struct into csv file)
- `external_definitions_person.c` (provide how to convert your struct into a string, call init api `csv_init()` and serialization api `v3()`)
