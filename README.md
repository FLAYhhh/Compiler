# Compiler
This is a Compiler project. Now I just finish the hand craft scanner.
grammar analysis part isn't here

# Lexical analysis

## File Hierarchy
- **bin**
     + **build**: 可执行文件.1.从regex.txt获取词法规则 2.从sourfile获取源程序 3.输出token序列.
- **file**
     + **dfaviz.dot** : 由build生成的dfa,描述方式为dot语言.
     + **dfaviz.jpg** : 使用graphviz程序,由dfaviz.dot生成的dfa图片.
     + **nfaviz.dot** : 由build生成的nfa,描述方式为dot语言.
     + **nfaviz.jpg** : 使用graphviz程序,由nfaviz.dot生成的nfa图片.
     + **regex.txt** : 描述语言的词法规则.
     + **sourcefile.txt** : 等待解析的源程序. 
     + **tokens.txt** : 生成的token序列.
     + **makefile** : 根据dfaviz.dot和nfaviz.dot生成相应的图片.
     + **其他** : 测试使用的文件,忽视即可.
- **obj**
     + ***.o** : 链接过程中生成的目标文件.
- **src**
     + **fa_struct.h** : 声明了nfa和dfa的相关结构和函数.
     + **fa_struct.c** : 定义了fa_struct.h中声明的内容.
     + **regex_engine.h** : 声明了将**单个**正则表达式转换为nfa和dfa的函数.
     + **regex_engine.c** : 定义了regex_engine.h中声明的内容.
     + **scanner_generator.h**: 1.声明了将正则表达式**集合**转化为dfa的函数 ;2.声明了词法分析函数.
     + **scanner_generator.c**:定义了scanner_generator.h中的声明. 
     + **lexical.c**: 包含主函数,完成词法分析.

- **makefile** : 编译链接
