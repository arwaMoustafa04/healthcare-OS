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
        # check if the id already exists
        if ls "$STORAGE" | grep -q "^$ID"; then
            echo "[OS Shell] Error: A patient with ID $ID already exists!"
        else
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
        fi
        ;;
    
    "view")
        # Your original code... (cat the file)
        FILE_PATH="$STORAGE/$2/info.txt"
        if [ -f "$FILE_PATH" ]; then
            cat "$FILE_PATH"
        else
            echo "[OS Shell] Error: Record for '$2' not found."
        fi
        ;;

    "delete")
        # Your original code... (rm the folder)
        if [ -d "$STORAGE/$2" ]; then
            rm -rf "$STORAGE/$2"
            echo "[OS Shell] Record '$2' has been deleted successfully."
        else
            echo "[OS Shell] Error: Cannot delete. Record not found."
        fi
        ;;
        
    "list")
        # Your original code... (ls the folder)
        echo "--- Hospital Patient Directories ---"
        if [ "$(ls -A $STORAGE 2>/dev/null)" ]; then
            ls -1 "$STORAGE"
        else
            echo "The patient list is currently empty."
        fi
        echo "------------------------------------"
        ;;
esac