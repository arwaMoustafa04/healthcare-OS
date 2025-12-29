#!/bin/bash

STORAGE="Patients"

COMMAND=$1
ID=$2
NAME=$3
AGE=$4       
CONDITION=$5 
mkdir -p "$STORAGE"

case $COMMAND in
    "create")
        FOLDER="$STORAGE/${ID}_${NAME}"
        mkdir -p "$FOLDER"
        echo "--------------------------------" > "$FOLDER/info.txt"
        echo "PATIENT MEDICAL RECORD" >> "$FOLDER/info.txt"
        echo "--------------------------------" >> "$FOLDER/info.txt"
        echo "ID: $ID" >> "$FOLDER/info.txt"
        echo "Name: $NAME" >> "$FOLDER/info.txt"
        echo "Age: $AGE" >> "$FOLDER/info.txt"
        echo "Condition: $CONDITION" >> "$FOLDER/info.txt"
        echo "--------------------------------" >> "$FOLDER/info.txt"
        
        echo "[OS Shell] Directory has been created successfully."
        ;;
    
    "view")
        FILE_PATH="$STORAGE/$2/info.txt"
        
        if [ -f "$FILE_PATH" ]; then
            cat "$FILE_PATH"
        else
            echo "[OS Shell] Error: Record for '$2' not found."
        fi
        ;;

    "delete")
        if [ -d "$STORAGE/$2" ]; then
            rm -rf "$STORAGE/$2"
            echo "[OS Shell] Record '$2' has been deleted successfully."
        else
            echo "[OS Shell] Error: Cannot delete. Record not found."
        fi
        ;;
        
    "list")
        echo "--- Hospital Patient Directories ---"
        if [ "$(ls -A $STORAGE 2>/dev/null)" ]; then
            ls -1 "$STORAGE"
        else
            echo "The patient list is currently empty."
        fi
        echo "------------------------------------"
        ;;
esac
