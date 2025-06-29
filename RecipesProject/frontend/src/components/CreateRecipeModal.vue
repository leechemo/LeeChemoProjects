<template>
  <div class="modal fade show d-block" tabindex="-1" style="background-color: rgba(0,0,0,0.5);">
    <div class="modal-dialog modal-lg">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title"><i class="bi bi-plus-circle me-2"></i> Create New Recipe</h5>
          <button @click="closeModal" type="button" class="btn-close"></button>
        </div>

        <div class="modal-body">
          <form @submit.prevent="submitRecipe">
            <div class="row">

              <!-- Title -->
              <div class="col-md-6 mb-3">
                <label class="form-label">Recipe Title *</label>
                <input v-model="form.title" type="text" class="form-control" required />
              </div>

              <!-- Ready In Minutes -->
              <div class="col-md-6 mb-3">
                <label class="form-label">Preparation Time (minutes) *</label>
                <input v-model.number="form.readyInMinutes" type="number" min="1" class="form-control" required />
              </div>

              <!-- Servings -->
              <div class="col-md-6 mb-3">
                <label class="form-label">Servings *</label>
                <input v-model.number="form.servings" type="number" min="1" class="form-control" required />
              </div>

              <!-- Image URL -->
              <div class="col-md-6 mb-3">
                <label class="form-label">Image URL</label>
                <input v-model="form.image" type="url" class="form-control" placeholder="https://example.com/image.jpg" />
              </div>

              <!-- Dietary -->
              <div class="col-12 mb-3">
                <label class="form-label">Dietary Options</label>
                <div class="form-check form-check-inline">
                  <input v-model="form.vegetarian" type="checkbox" class="form-check-input" />
                  <label class="form-check-label">Vegetarian</label>
                </div>
                <div class="form-check form-check-inline">
                  <input v-model="form.vegan" type="checkbox" class="form-check-input" />
                  <label class="form-check-label">Vegan</label>
                </div>
                <div class="form-check form-check-inline">
                  <input v-model="form.glutenFree" type="checkbox" class="form-check-input" />
                  <label class="form-check-label">Gluten-Free</label>
                </div>
              </div>

              <!-- Is Family Recipe -->
              <div class="col-12 mb-3">
                <div class="form-check">
                  <input v-model="form.isFamily" type="checkbox" class="form-check-input" id="familyCheckbox" />
                  <label class="form-check-label" for="familyCheckbox">This is a family recipe</label>
                </div>
              </div>

              <!-- Family Fields -->
              <div v-if="form.isFamily" class="row">
                <div class="col-md-6 mb-3">
                  <label class="form-label">Family Member *</label>
                  <input v-model="form.familyMember" type="text" class="form-control" required />
                </div>
                <div class="col-md-6 mb-3">
                  <label class="form-label">Occasion *</label>
                  <input v-model="form.occasion" type="text" class="form-control" required />
                </div>
              </div>

              <!-- Ingredients -->
              <div class="col-12 mb-3">
                <label class="form-label">Ingredients *</label>
                <div v-for="(ingredient, index) in form.ingredients" :key="index" class="input-group mb-2">
                  <input v-model="ingredient.name" type="text" placeholder="Ingredient name" class="form-control" required />
                  <input v-model="ingredient.amount" type="text" placeholder="Amount" class="form-control" required />
                  <input v-model="ingredient.unit" type="text" placeholder="Unit" class="form-control" required />
                  <button type="button" class="btn btn-outline-danger" @click="removeIngredient(index)">
                    <i class="bi bi-trash"></i>
                  </button>
                </div>
                <button type="button" class="btn btn-outline-primary btn-sm" @click="addIngredient">
                  <i class="bi bi-plus"></i> Add Ingredient
                </button>
              </div>

              <!-- Instructions -->
              <div class="col-12 mb-3">
                <label class="form-label">Instructions *</label>
                <textarea v-model="form.instructions" rows="5" class="form-control" required></textarea>
              </div>

            </div>
          </form>
        </div>

        <div class="modal-footer">
          <button @click="closeModal" type="button" class="btn btn-secondary">Cancel</button>
          <button @click="submitRecipe" type="button" class="btn btn-primary" :disabled="submitting">
            <span v-if="submitting" class="spinner-border spinner-border-sm me-2"></span>
            Create Recipe
          </button>
        </div>

      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import store from '@/store';

export default {
  name: 'CreateRecipeModal',
  props: ['onSuccess', 'onClose'],
  data() {
    return {
      submitting: false,
      form: {
        title: '',
        readyInMinutes: 30,
        servings: 4,
        image: '',
        vegetarian: false,
        vegan: false,
        glutenFree: false,
        isFamily: false,
        familyMember: '',
        occasion: '',
        ingredients: [{ name: '', amount: '', unit: '' }],
        instructions: ''
      }
    };
  },
  methods: {
    addIngredient() {
      this.form.ingredients.push({ name: '', amount: '', unit: '' });
    },
    removeIngredient(index) {
      if (this.form.ingredients.length > 1) {
        this.form.ingredients.splice(index, 1);
      }
    },
    closeModal() {
      if (this.onClose) this.onClose();
    },
    async submitRecipe() {
      if (!this.validateForm()) return;
      this.submitting = true;

      const requestBody = {
        ...this.form,
        instructionsSteps: this.form.instructions
          .split('\n')
          .filter(step => step.trim() !== '')
          .map((desc, idx) => ({
            stepNumber: idx + 1,
            description: desc,
            completed: false
          }))
      };

      try {
        await axios.post(`${store.server_domain}/user/recipes`, requestBody, { withCredentials: true });
        if (this.onSuccess) this.onSuccess();
        this.closeModal();
      } catch (error) {
        console.error('Error creating recipe:', error);
        alert('Failed to create recipe.');
      } finally {
        this.submitting = false;
      }
    },
    validateForm() {
      if (!this.form.title.trim() || !this.form.instructions.trim()) {
        alert('Please fill all required fields.');
        return false;
      }
      const validIngredients = this.form.ingredients.filter(ing => ing.name.trim() && ing.amount.trim());
      if (validIngredients.length === 0) {
        alert('Please provide at least one ingredient.');
        return false;
      }
      if (this.form.isFamily && (!this.form.familyMember.trim() || !this.form.occasion.trim())) {
        alert('Please fill family member and occasion fields for family recipe.');
        return false;
      }
      return true;
    }
  }
};
</script>

<style scoped>
.modal {
  z-index: 1050;
}

.form-check {
  margin-bottom: 0.5rem;
}

.btn {
  border-radius: 8px;
  font-weight: 500;
}

.btn:hover {
  transform: translateY(-1px);
}
</style>
