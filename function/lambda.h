#ifndef LAMBDA_H_
    #define LAMBDA_H_
        #define __lambda__(return_type, function_body) \
            ({ \
            return_type __fn__ function_body; \
            __fn__; \
            })
        #define lambda __lambda__
#endif /* !LAMBDA_H_ */
