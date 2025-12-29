#!/bin/bash

BASE_DIR="$HOME/hospital_data"
PATIENT_DIR="$BASE_DIR/patients"
ARCHIVE_DIR="$BASE_DIR/archive"

echo "[OS Shell] Initializing Healthcare File System..."

mkdir -p "$PATIENT_DIR"
mkdir -p "$ARCHIVE_DIR"

chmod 700 "$BASE_DIR"

echo "[OS Shell] Directory structure verified at $BASE_DIR"
