<template>
  <div class="container-fluid py-4">
    <div class="row justify-content-center">
      <div class="col-lg-10">

        <!-- Loading State -->
        <div v-if="loading" class="text-center py-5">
          <div class="spinner-border text-primary mb-3" role="status">
            <span class="visually-hidden">Loading...</span>
          </div>
          <p class="text-muted">Loading recipe details...</p>
        </div>

        <!-- Recipe Details -->
        <div v-else-if="recipe" class="recipe-detail-container">

          <!-- Header -->
          <div class="row mb-4">
            <div class="col-md-6">
              <img :src="recipe.image" :alt="recipe.title" class="img-fluid rounded recipe-main-image" />
            </div>
            <div class="col-md-6">
              <h1 class="display-5 text-primary mb-3">{{ recipe.title }}</h1>

              <div class="recipe-meta mb-3">
                <span class="badge bg-primary me-2"><i class="bi bi-clock"></i> {{ recipe.readyInMinutes }} min</span>
                <span class="badge bg-success me-2"><i class="bi bi-star"></i> {{ recipe.aggregateLikes || recipe.popularity }} likes</span>
                <span class="badge bg-info"><i class="bi bi-people"></i> {{ recipe.servings }} servings</span>
              </div>

              <div class="dietary-indicators mb-3">
                <span v-if="recipe.vegan" class="badge bg-success me-2"><i class="bi bi-leaf"></i> Vegan</span>
                <span v-else-if="recipe.vegetarian" class="badge bg-info me-2"><i class="bi bi-flower1"></i> Vegetarian</span>
                <span v-if="recipe.glutenFree" class="badge bg-warning"><i class="bi bi-shield-check"></i> Gluten-Free</span>
              </div>

              <div class="action-buttons mb-3">
                <button @click="toggleFavorite" class="btn me-2" :class="isFavorite ? 'btn-danger' : 'btn-outline-danger'">
                  <i class="bi" :class="isFavorite ? 'bi-heart-fill' : 'bi-heart'"></i>
                  {{ isFavorite ? 'Remove from Favorites' : 'Add to Favorites' }}
                </button>

                <!-- Start Preparing -->
                <router-link :to="getPrepareLink()" class="btn btn-success" v-if="showPrepareButton">
                  <i class="bi bi-play-circle me-2"></i>Start Preparing
                </router-link>

                <!-- Add to Meal Plan -->
                <button @click="addToMealPlan" class="btn btn-outline-success">
                  <i class="bi bi-plus-circle me-2"></i> Add to Meal Plan
                </button>
              </div>

              <p class="text-muted" v-html="recipe.instructions"></p>
            </div>
          </div>

          <!-- Ingredients -->
          <div class="row">
            <div class="col-md-4 mb-4">
              <div class="card shadow-sm">
                <div class="card-header bg-primary text-white">
                  <h5><i class="bi bi-list-ul me-2"></i> Ingredients</h5>
                </div>
                <div class="card-body">
                  <ul class="list-group list-group-flush">
                    <li v-for="ingredient in recipe.ingredients" :key="ingredient.ingredientName || ingredient.name"
                      class="list-group-item d-flex justify-content-between align-items-center">
                      <span>{{ ingredient.ingredientName || ingredient.name}}</span>
                      <span class="badge bg-light text-dark">{{ formatAmount(ingredient) }}</span>
                    </li>
                  </ul>
                </div>
              </div>
            </div>

            <!-- Preparation Steps -->
            <!-- <div class="col-md-8 mb-4" v-if="recipe.instructionsSteps && recipe.instructionsSteps.length">
              <div class="card shadow-sm">
                <div class="card-header bg-success text-white">
                  <h5><i class="bi bi-list-ol me-2"></i> Preparation Steps</h5>
                </div>
                <div class="card-body">
                  <ol>
                    <li v-for="step in recipe.instructionsSteps" :key="step.stepNumber" class="mb-2">
                      {{ step.description }}
                    </li>
                  </ol>
                </div>
              </div>
            </div> -->

          </div>
        </div>

        <!-- Recipe Not Found -->
        <div v-else class="text-center py-5">
          <div class="card shadow-sm">
            <div class="card-body py-5">
              <i class="bi bi-exclamation-triangle display-1 text-warning mb-3"></i>
              <h3 class="text-muted mb-3">Recipe not found</h3>
              <router-link to="/search" class="btn btn-primary btn-lg">
                <i class="bi bi-search me-2"></i> Search Recipes
              </router-link>
            </div>
          </div>
        </div>

      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import store from '@/store';

export default {
  name: 'RecipeViewPage',
  data() {
    return {
      loading: true,
      recipe: null,
      recipeType: null
    };
  },
  mounted() {
    this.determineRecipeType();
    this.loadRecipe();
  },

  watch: {
    '$route'() {
      this.determineRecipeType();
      this.loadRecipe();
    }
  },
  methods: {
    // determineRecipeType() {
    //   const path = this.$route.path.toLowerCase();
    //   if (path.includes('/my-recipes/')) {
    //     this.recipeType = 'personal';
    //   } else if (path.includes('/family-recipes/')) {
    //     this.recipeType = 'family';
    //   } else if (path.includes('/recipes/fullview/')) {
    //     this.recipeType = 'spoonacular';
    //   }
    // },

    determineRecipeType() {
      const routeName = this.$route.name;
      if (routeName === 'MyRecipeView') {
        this.recipeType = 'personal';
      } else if (routeName === 'FamilyRecipeView') {
        this.recipeType = 'family';
      } else if (routeName === 'FullRecipeView') {
        this.recipeType = 'spoonacular';
      }
    },

    async loadRecipe() {
      this.loading = true;
      try {
        const recipeID = this.$route.params.recipeID;
        let response;
        if (this.recipeType === 'family') {
          response = await axios.get(`${store.server_domain}/user/family-recipes/${recipeID}`, { withCredentials: true });
        } else if (this.recipeType === 'personal') {
          response = await axios.get(`${store.server_domain}/user/my-recipes/${recipeID}`, { withCredentials: true });
        } else if (this.recipeType === 'spoonacular') {
          response = await axios.get(`${store.server_domain}/recipes/fullview/${recipeID}`, { withCredentials: true });
        }
        this.recipe = response.data;

        if (this.recipe.instructions) {
          this.recipe.instructions = this.recipe.instructions
            .replace(/<\/?ol>/g, '')
            .replace(/<\/?li>/g, '<br>');
        }

      } catch (error) {
        console.error('Error loading recipe:', error);
        this.recipe = null;
      } finally {
        this.loading = false;
      }
    },

    getPrepareLink() {
      const recipeID = this.$route.params.recipeID;
      if (this.recipeType === 'family') {
        return `/user/family-recipes/${recipeID}/prepare`;
      } else if (this.recipeType === 'personal') {
        return `/user/my-recipes/${recipeID}/prepare`;
      } else {
        return `/recipes/${recipeID}/prepare`;
      }
    },

    async toggleFavorite() {
      try {
        const recipeID = this.recipe.recipeID || this.recipe.recipeId;  
        await axios.post(`${store.server_domain}/user/favorites`, 
          { recipeID: recipeID },
          { withCredentials: true }
        );
        this.recipe.isFavorite = true;  
        this.toast("Success", "Recipe added to favorites!", "success");
      } catch (error) {
        console.error('Error adding to favorites:', error);
        this.toast("Error", "Failed to add to favorites", "danger");
      }
    },



    async addToMealPlan() {
      try {
        const recipeID = (this.recipe.recipeID).toString();  
        await axios.post(`${store.server_domain}/user/meal-plan`, { recipeID }, { withCredentials: true });
        alert('Recipe added to meal plan.');
      } catch (error) {
        console.error('Error adding to meal plan:', error);
        alert('Failed to add to meal plan.');
      }
    },

    formatAmount(ingredient) {
      if (ingredient.unit) {
        return `${ingredient.amount} ${ingredient.unit}`;
      }
      return ingredient.amount;
    }
  },

  computed: {
    showPrepareButton() {
      return this.recipeType !== null;
    },
    isFavorite() {
      return this.recipe?.isFavorite === true;
    }
  },

};
</script>

<style scoped>
.recipe-detail-container {
  max-width: 1200px;
  margin: 0 auto;
}
.recipe-main-image {
  width: 100%;
  height: 400px;
  object-fit: cover;
  border-radius: 15px;
}
.recipe-meta, .dietary-indicators, .action-buttons {
  display: flex;
  flex-wrap: wrap;
  gap: 0.5rem;
}
.card {
  border: none;
  border-radius: 15px;
}
.card-header {
  border-radius: 15px 15px 0 0 !important;
  border-bottom: none;
}
.btn {
  border-radius: 8px;
  font-weight: 500;
}
.btn:hover {
  transform: translateY(-1px);
}
.badge {
  font-size: 0.75rem;
  padding: 0.25rem 0.5rem;
}
</style>
