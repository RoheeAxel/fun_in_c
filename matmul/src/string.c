#include "true_string.h"

const str _str__repr__(str self) {
    return self;
}

/*
This function concatenates two strings and returns the result

@param self: The first string
@param other: The second string

@return: The concatenated string in a new memory location
*/
str _str__add__(str self, str other) {
    int _result_size = self->size + other->size;
    char *_result_data = (char *)malloc(_result_size * sizeof(char) + 1);
    if (_result_data == NULL) {
        WARNING("Cannot allocate memory for string concatenation");
        return NULL;
    }
    strcpy(_result_data, self->data);
    strcat(_result_data, other->data);
    _result_data[_result_size] = '\0';
    self->size = _result_size;
    self->data = _result_data;
    return self;
}

/*
This function returns the length of a string

@param self: The string

@return: The length of the string object
*/
int _str__len__(str self) {
    return self->size;
}

/*
This function is the basic constuctor called by default when using String Macro

@return: A new string object with NULL data
*/
str string_ctor() {
    str s = malloc(sizeof(str_t));
    if (s == NULL) {
        WARNING("Cannot allocate memory for string object");
        return NULL;
    }
    s->__class__ = "String";
    s->__type__ = NULL;
    s->size = 0;
    s->data = NULL;
    s->__repr__ = _str__repr__;
    s->__iadd__ = _str__add__;
    s->__len__ = _str__len__;
    return s;
}

/*
This function creates a new string object from a char array it is called by the String Macro

@param data: The char array to be converted to a string

@return: A new string object with the char array as data
*/
str _string_from_char(char *data) {
    str s = string_ctor();
    if (s == NULL) {
        WARNING("Cannot create string object from char");
        return NULL;
    }
    s->size = strlen(data);
    s->data = (char *)malloc(s->size * sizeof(char) + 1);
    if (s->data == NULL) {
        WARNING("Cannot allocate memory for data of string object from char");
        return NULL;
    }
    strcpy(s->data, data);
    s->data[s->size] = '\0';
    return s;
}

/*
This function creates a new string object from an integer it is called by the String Macro

@param data: The integer to be converted to a string

@return: A new string object with the integer converted to a string
*/
str _string_from_int(int data) {
    char buffer[20];
    sprintf(buffer, "%d", data);
    return _string_from_char(buffer);
}