" Vim syntax file
" Language:     Om
" Maintainer:   Michael Emch   
" Updaters:     
" URL:          
" Changes:      
" Last Change:  2010 Dec 13

if !exists("main_syntax")
  if version < 600
    syntax clear
  elseif exists("b:current_syntax")
    finish
  endif
  let main_syntax = 'om'
endif

syn match   omComment           "#.*" 
syn match   omInteger           "\d+"
syn match   omFloat             "\d*\.\d+"
syn match   omString            "\"[^\"]*\""
syn match   omType              "[A-Z]\w*"

syn keyword omConditional       if or else fi 
syn keyword omLoop              for fin
syn keyword omBranch            brk con ret end
syn keyword omInclude           inc def prt
syn keyword omOperator          = + - * / % == != < <= > >= ! += -= *= /= %= ^ _ &

hi def link omComment           Comment
hi def link omInteger           Number
hi def link omFloat             Number
hi def link omString            String
hi def link omType              Type
hi def link omConditional       Conditional
hi def link omLoop              Repeat
hi def link omBranch            Conditional
hi def link omInclude           Include
hi def link omOperator          Operator

let b:current_syntax = "om"
if main_syntax == 'om'
  unlet main_syntax
endif
