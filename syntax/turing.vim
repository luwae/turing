" Vim syntax file
" Language: Turing Machine Source

if exists("b:current_syntax")
    finish
endif

syn match turingSym '^.:'
syn match turingComment '#.*$'
syn keyword turingKeywords state end

let b:current_syntax = "turing"

hi def link turingKeywords Statement
hi def link turingSym Constant
hi def link turingComment Comment
