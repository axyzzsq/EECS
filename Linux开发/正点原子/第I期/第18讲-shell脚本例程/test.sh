#!/bin/bash
echo "intput filename:"
read -p "filename:" filename
test -e $filename && echo "$filename 存在" || echo "$filename 不存在"
