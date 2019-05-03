# -*- coding: utf-8 -*-
from pygments.style import Style
from pygments.token import Keyword, Name, Comment, Operator, Generic, Literal, Text, String

# All the solarized colors, in case I want to use more of those later
BASE0 = '#839496'
BASE1 = '#93a1a1'
BASE2 = '#eee8d5'
BASE3 = '#fdf6e3'
BASE00 = '#657b83'
BASE01 = '#586e75'
BASE02 = '#073642'
BASE03 = '#002b36'
YELLOW = '#b58900'
ORANGE = '#cb4b16'
RED = '#dc322f'
MAGENTA = '#d33682'
VIOLET = '#6c71c4'
BLUE = '#268bd2'
CYAN = '#2aa198'
GREEN = '#859900'


class MysolarizedStyle(Style):

    """ My Solarized Light based on http://ethanschoonover.com/solarized. """
    background_color = BASE2
    styles = {
        Text: 'bg: %s %s' % (BASE2, BASE00),
        Keyword: YELLOW,
        Keyword.Type: RED,
        Name: BASE00,
        Name.Function: BLUE,
        Name.Class: BLUE,
        Name.Namespace: BLUE,
        Name.Builtin.Pseudo: 'bold %s' % BLUE,
        Name.Constant: RED,
        Literal: ORANGE,
        String.Doc: BASE1,
        Comment: BASE1,
        Operator: BASE00
    }
